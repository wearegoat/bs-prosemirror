module Types = PM_Types;

type pos = {
  .
  "top": int,
  "right": int,
  "bottom": int,
  "left": int,
};

[@bs.deriving abstract]
type t = {
  [@bs.optional]
  handleDOMEvents: Js.Dict.t((~view: Types.editorView, ~event: Dom.event) => bool),
  [@bs.optional]
  handleKeyDown: (~view: Types.editorView, ~event: Dom.keyboardEvent) => bool,
  [@bs.optional]
  handleKeyPress: (~view: Types.editorView, ~event: Dom.keyboardEvent) => bool,
  [@bs.optional]
  handleTextInput: (~view: Types.editorView, ~from: int, ~to_: int, ~text: string) => bool,
  [@bs.optional]
  handleClickOn:
    (
      ~view: Types.editorView,
      ~pos: int,
      ~node: PM_Model.Node.t,
      ~nodePos: int,
      ~event: Dom.mouseEvent,
      ~direct: bool
    ) =>
    bool,
  [@bs.optional]
  handleClick: (~view: Types.editorView, ~pos: int, ~event: Dom.mouseEvent) => bool,
  [@bs.optional]
  handleDoubleClickOn:
    (
      ~view: Types.editorView,
      ~pos: int,
      ~node: PM_Model.Node.t,
      ~nodePos: int,
      ~event: Dom.mouseEvent,
      ~direct: bool
    ) =>
    bool,
  [@bs.optional]
  handleDoubleClick: (~view: Types.editorView, ~pos: int, ~event: Dom.mouseEvent) => bool,
  [@bs.optional]
  handleTripleClickOn:
    (
      ~view: Types.editorView,
      ~pos: int,
      ~node: PM_Model.Node.t,
      ~nodePos: int,
      ~event: Dom.mouseEvent,
      ~direct: bool
    ) =>
    bool,
  [@bs.optional]
  handleTripleClick: (~view: Types.editorView, ~pos: int, ~event: Dom.mouseEvent) => bool,
  [@bs.optional]
  handlePaste: (~view: Types.editorView, ~event: Dom.event, ~slice: PM_Model.Slice.t) => bool,
  [@bs.optional]
  handleDrop:
    (~view: Types.editorView, ~event: Dom.event, ~slice: PM_Model.Slice.t, ~moved: bool) => bool,
  [@bs.optional]
  handleScrollToSelection: Types.editorView => bool,
  [@bs.optional]
  createSelectionBetween:
    (~view: Types.editorView, ~anchor: PM_Model.ResolvedPos.t, ~head: PM_Model.ResolvedPos.t) =>
    option(PM_Types.selection),
  /* TODO: */
  /* [@bs.optional] */
  /* domParser: PM_Model.DOMParser.t, */
  [@bs.optional]
  transformPastedHTML: (~html: string) => string,
  /* TODO: */
  /* [@bs.optional] */
  /* clipboardParser: PM_Model.DOMParser.t, */
  [@bs.optional]
  transformPastedText: (~text: string) => string,
  [@bs.optional]
  clipboardTextParser: (~text: string, ~context: PM_Model.ResolvedPos.t) => PM_Model.Slice.t,
  [@bs.optional]
  transformPasted: PM_Model.Slice.t => PM_Model.Slice.t,
  [@bs.optional]
  nodeViews:
    Js.Dict.t(
      (
        ~node: PM_Model.Node.t,
        ~view: Types.editorView,
        ~getPos: unit => int,
        ~decorations: array(PM_Decoration.t)
      ) =>
      PM_NodeView.t,
    ),
  [@bs.optional]
  clipboardSerializer: PM_Model.DOMSerializer.t,
  [@bs.optional]
  clipboardTextSerializer: PM_Model.Slice.t => string,
  [@bs.optional]
  decorations: PM_Types.editorState => Types.decorationSet,
  [@bs.optional]
  editable: PM_Types.editorState => bool,
  [@bs.optional]
  attributes: Js.Dict.t(string),
  [@bs.optional] [@bs.as "attributes"]
  attributesFn: PM_Types.editorState => Js.Nullable.t(Js.Dict.t(string)),
  [@bs.optional]
  scrollThreshold: int,
  [@bs.optional] [@bs.as "scrollThreshold"]
  scrollThresholdObj: pos,
  [@bs.optional]
  scrollMargin: int,
  [@bs.optional] [@bs.as "scrollMarginObj"]
  scrollMarginObj: pos,
};
