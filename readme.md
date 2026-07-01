# Young examples
These directories contain the source to famous book "The X Window System:
Programming and Applications with Xt, Second Motif Edition", by
Douglas Young, Prentice Hall, 1994

Original README file: [README](README)

I went through all files and made minor changes to make the code compile and the examples work
on a nowadays Linux (2026, Opensuse Leap 15.6, gcc 15.2.0)

## Issues
* ch17/Tree.c, changed API:
```c++
/*
* Move the widget into position.
*/
// _XmMoveObject is gone in 2.1, use XmeConfigureObject(), but API differs - needs analysis
_XmMoveObject ( w, tree_const->tree.x, tree_const->tree.y );
```

## Further reading
* Location of tar.Z file https://ftp.gwdg.de/pub/x11/x.org/R5contrib/young.motif2.tar.Z   