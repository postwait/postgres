/* contrib/spi/timetravel--1.0.sql */

CREATE OR REPLACE FUNCTION timetravel()
RETURNS trigger
AS 'MODULE_PATHNAME'
LANGUAGE C;

CREATE OR REPLACE FUNCTION set_timetravel(name, int4)
RETURNS int4
AS 'MODULE_PATHNAME'
LANGUAGE C RETURNS NULL ON NULL INPUT;

CREATE OR REPLACE FUNCTION get_timetravel(name)
RETURNS int4
AS 'MODULE_PATHNAME'
LANGUAGE C RETURNS NULL ON NULL INPUT;