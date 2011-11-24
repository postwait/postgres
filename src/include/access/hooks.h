/*-------------------------------------------------------------------------
 *
 * hooks.h
 *    POSTGRES hooks.
 *
 *
 * Portions Copyright (c) 2011, PostgreSQL Global Development Group
 *
 * src/include/access/hooks.h
 *
 *-------------------------------------------------------------------------
 */
#ifndef HOOKS_H
#define HOOKS_H

#include "nodes/pg_list.h"

/*
 * Allow arbitrary logging of queries.
 */
typedef enum {
	LOG_HOOK_CONTEXT_SIMPLE = 0,
  LOG_HOOK_CONTEXT_BIND = 1,
	LOG_HOOK_CONTEXT_PORTAL = 2,
  LOG_HOOK_CONTEXT_EXEC_PARSE = 3,
  LOG_HOOK_CONTEXT_FASTPATH = 4
} LogHookContext;

typedef void (*pre_exec_log_hook_t)(LogHookContext ctx, const char *query,
							List *parsetree, bool was_logged);
typedef void (*post_exec_log_hook_t)(LogHookContext ctx, const char *query,
							List *parsetree, bool was_logged, long usec_duration);

/*
 * These functions can be used by modules authors to instrument the
 * logging process and do arbitrary pre-flight and post-flight logging
 * of statements.
 */
void	register_pre_exec_log_hook(pre_exec_log_hook_t f);
void	register_post_exec_log_hook(post_exec_log_hook_t f);

/*
 * Call the hooks
 */
void	call_pre_exec_log_hooks(LogHookContext ctx, const char *query,
					List *tree, bool was_logged);
void	call_post_exec_log_hooks(LogHookContext ctx, const char *query,
					List *tree, bool was_logged);
#endif
