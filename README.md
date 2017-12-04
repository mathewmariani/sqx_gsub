# Squirrel Extension Global Substitution
A small extension for the Squirrel Programming Language for string replacement; inspired by Lua and Ruby.

## Usage

**[sqx_gsub.cpp](src/sqx_gsub.cpp?raw=1)** and **[sqx_gsub.h](src/sqx_gsub.h?raw=1)** should be dropped
into an existing project — does not require any standard squirrel libraries to be registered.

Used to replace all instances of a pattern with a replacement. Returns a copy of str with the all occurrences of `p` replaced with `r`.

### gsub(str, p, r)

```
$ gsub("Hello, World", "World", "Squirrel");
> Hello, Squirrel
```

## License
This library is free software; you can redistribute it and/or modify it under
the terms of the MIT license. See [LICENSE](LICENSE) for details.