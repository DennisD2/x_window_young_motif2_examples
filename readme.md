# Young examples

Original README file: [README](README)
## Issues
* ch17/Tree.c, changed API:
```c++
* Move the widget into position.
*/
// _XmMoveObject is gone in 2.1, use XmeConfigureObject(), but API differs - needs analysis
_XmMoveObject ( w, tree_const->tree.x, tree_const->tree.y );
```