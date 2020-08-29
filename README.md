This is a 3D raytracer only capable of rendering triangles, but the image is rendered in ascii characters in your terminal.  
Who asked? No one

Here's a preview of it (the default square based pyramid):  
```
                          =
                        , * *
                        , * *
                      , , * * *
                    , , , # # # #
                    , , # # # # # #
                  , , , # # # # # #
                , , , , # # # # # # #
                , , , , # # # $ $ $ # #
              , , , , , # $ $ $ $ $ $ $ #
            , , , , , , $ $ $ $ $ $ $ $ $
            , , , , , $ $ $ $ $ $ $ $ $ $ $
          , , , , , , $ $ $ $ $ $ $ $ $ $ $ $
            , , , , , $ $ $ $ $ $ $ $ $ $ $ $
              , , , , $ $ $ $ $ $ $ $ $ $ $
              , , , , $ $ $ $ $ $ $ $
                , , , $ $ $ $ $ $
                  , $ $ $ $
                    $
```

### To Build:
You need the X11 headers, as keyboard input uses these.
On Arch Linux, these are provided by the `libx11` package.  
To compile, `./build.sh`

