# jlWU
stands for "[jorkdir](https://github.com/RixInGithub/jorkdir/) lua WITH UTILS"

## usage
### `require("jlWU")(dir?)`
returns a table with the raw results from the c jorkdir function, though the output might be reordered due to lua's shenanigans.

if `dir` is not provided, uses `.` instead.
### `jlWU.fullPath(p?)`
turns a relative path (relative to current directory of course) and turns it into an absolute path.

if `p` is not provided, uses `.` instead.
### `jlWU.isDir(p)`
takes in a path (can be relative or absolute) (__**REQUIRED**__).

if it doesn't exist or if it isn't a directory, returns `false`, otherwise returns `true`.

if `p` is not provided, throws an error.