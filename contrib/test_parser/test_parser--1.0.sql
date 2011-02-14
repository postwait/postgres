/* contrib/test_parser/test_parser--1.0.sql */

CREATE OR REPLACE FUNCTION testprs_start(internal, int4)
RETURNS internal
AS 'MODULE_PATHNAME'
LANGUAGE C STRICT;

CREATE OR REPLACE FUNCTION testprs_getlexeme(internal, internal, internal)
RETURNS internal
AS 'MODULE_PATHNAME'
LANGUAGE C STRICT;

CREATE OR REPLACE FUNCTION testprs_end(internal)
RETURNS void
AS 'MODULE_PATHNAME'
LANGUAGE C STRICT;

CREATE OR REPLACE FUNCTION testprs_lextype(internal)
RETURNS internal
AS 'MODULE_PATHNAME'
LANGUAGE C STRICT;

CREATE TEXT SEARCH PARSER testparser (
    START    = testprs_start,
    GETTOKEN = testprs_getlexeme,
    END      = testprs_end,
    HEADLINE = pg_catalog.prsd_headline,
    LEXTYPES = testprs_lextype
);