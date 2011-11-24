/* -------------------------------------------------------------------------
 *
 * pipe_log.c
 *
 * Copyright (C) 2011, OmniTI Labs
 *
 * IDENTIFICATION
 *		contrib/pipe_log/auth_delay.c
 *
 * -------------------------------------------------------------------------
 */
#include "postgres.h"

#include "port.h"
#include "utils/guc.h"
#include "utils/timestamp.h"
#include "access/hooks.h"

#include <unistd.h>
#include <sys/stat.h>
#include <sys/uio.h>
#include <errno.h>

PG_MODULE_MAGIC;

void		_PG_init(void);

/* GUC Variables */
static char	*pipe_filename = "/tmp/.pg_pipe_log";

/* File descriptor */
static int	pipe_fd = -1;

/*
 * Check authentication
 */
static void
initalize_pipe(const char *filename)
{
  int rv;
  struct stat sb;
  if(pipe_fd == -1) {
    while((rv = stat(filename, &sb)) == -1 && errno == EINTR);
    if(rv == 0) {
      if(!S_ISFIFO(sb.st_mode)) {
        elog(ERROR, "%s is not a fifo.", filename);
        return;
      }
    }
    else {
      if(mkfifo(filename, 0640) < 0) {
        elog(FATAL, "cannot make fifo %s: %s", filename, strerror(errno));
        return;
      }
    }
    pipe_fd = open(filename, O_WRONLY|O_NONBLOCK);
    if(pipe_fd < 0) {
      if(errno != ENXIO)
        elog(ERROR, "cannot open fifo %s: %s", filename, strerror(errno));
      return;
    }
    if(fcntl(pipe_fd, F_GETFL, O_NONBLOCK) < 0) {
      elog(ERROR, "cannot non-block fifo %s: %s", filename, strerror(errno));
      close(pipe_fd);
      pipe_fd = -1;
    }
  }
}

static void
pipe_post_log(LogHookContext ctx, const char *query,
			List *parsetree, bool was_logged, long usec_duration)
{
#define MAX_LEN 32768
  initalize_pipe(pipe_filename);
  if(pipe_fd < 0) return;
  if(query) {
    int qlen = strlen(query);
    int len = 0;
    int dur = 0;
    struct iovec lb[3] =
        { { (char *)&len, sizeof(len) },
          { (char *)&dur, sizeof(dur) },
          { (char *)query, MAX_LEN - sizeof(len) - sizeof(dur) } };
    if(qlen < MAX_LEN - sizeof(len) - sizeof(dur))
      lb[2].iov_len = qlen;
    if(writev(pipe_fd, lb, 3) < 0) {
      if(errno == EINTR || errno == EAGAIN) return;
      close(pipe_fd);
      pipe_fd = -1;
    }
  }
}

/*
 * Module Load Callback
 */
void
_PG_init(void)
{
	/* Define custom GUC variables */
	DefineCustomStringVariable("pipe_log.filename",
			 "Filename of the logging pipe",
							NULL, &pipe_filename, NULL,
							PGC_SIGHUP, 0, NULL, NULL, NULL);

  initalize_pipe(pipe_filename);
	/* Install Hooks */
	register_post_exec_log_hook(pipe_post_log);
}
