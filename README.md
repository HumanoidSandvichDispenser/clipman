# Clipman

Clipman is an X11 tool for managing clipboard history. By itself, it does not
do any managing but copying clipboard history to the filesystem.

## Clipboard Files

Clipboard files are stored with the `*.clip` extension in the directory
`~/.cache/clipman/` by default. A corresponding `*.cliph` describes the target
of the clipboard content (image/png, UTF8_STRING, etc.). The names of the files
are the MD5 hashes of its contents. This is so if a selection is reused,
instead of reappearing in the history directory, the file's modification time
updates.

## Enabling/Disabling

You can toggle clipman with SIGUSR2, disable with SIGRTMIN+1, and enable with
SIGRMIN+2.

## Dependencies

* `xclip` --- available most distro's package repositories
* `clipnotify` --- in Arch official repo, or [clone from GitHub and compile it
  yourself](https://github.com/cdown/clipnotify)

Makefile CXX flags: `-lX11 -lXfixes -lssl -lcrypto`
