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

### Building

To build:

`make all`

To install:

`sudo make install`

To uninstall:

`sudo make uninstall`

To clean:

`make clean`

## Example Scripts/Programs

Copying clipboard content with Python and `fzf`.

```py
# TODO: add code
```

Viewing images in history with `sxiv`, written in bash.

```sh
# go to history directory
cd ~/.cache/clipman/

image_clips=()

for clip in *.clip; do
    # add clip to array if it is a png image
    target=$(cat ${clip}h)
    [[ "$target" == "image/png" ]] && image_clips+=( $clip )
done

# if we don't have any images in the array, then exit
[ ${#image_clips[@]} -lt 0 ] && (echo "no images in clipboard history"; exit 0)

# join array elements by newline and pipe into sxiv
printf '%s\n' "${image_clips[@]}" | sxiv -it # -i to read stdin, -t to start in thumbnail mode
```

Deleting old clipboard files if history reaches 128, written in C.

```c
// TODO: add code
```
