**noets** is a helper application for writing down notes in your terminal, best used with _vimwiki_ and _fzf_.

## Requirements:
 - git
 - vim (optional)
 - fzf (optional)

## Configuration:
See `noets -h` for more information, config file is in `.toml` format.
```toml
local_repository_path = "/home/John/Documents/Notes"
remote_repository_url = "https://github.com/John/Notes"
git_username = "John"
git_email = "John@gmail.com"
editor = "notepad++"
```

## Building:
```
  mkdir build/
  cd build/
  cmake --build .
```

## Installation:
```
  cd build/
  cmake ..
  sudo make install
```

## Uninstall:
```
  cd build/
  sudo make uninstall
```