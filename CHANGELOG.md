BrefAPI Changelog
=================

Head
----
*  Fix compilation under *nix systems, where minor() and
   major() are already defined in <sys/types.h> (issue
   [#6](https://github.com/bref/bref-api/issues/6)).
*  bref::status_codes : Add Undefined (0) value. It will be the default defined
   value in response header. Could be usefull to detect if another ContentHook
   has already supported the content generation.
*  [See diff](https://github.com/bref/bref-api/compare/v0.3...master)

v0.3
----
*  **Change IModule to AModule**.
*  Add getType function in BrefValue, set confType enum as public (issues
   [#1](https://github.com/bref/bref-api/pull/1),
   [#2](https://github.com/bref/bref-api/pull/2) and
   [#7](https://github.com/bref/bref-api/pull/7)).
*  Add missing double member variable / functions in BrefValue.
*  [See diff](https://github.com/bref/bref-api/compare/v0.2...v0.3)

v0.2 / v0.1
-----------
*  API elections