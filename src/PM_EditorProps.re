module Types = PM_Types;

type pos = {
  .
  "top": int,
  "right": int,
  "bottom": int,
  "left": int,
};

module Attributes = {
  type t =
    | Dict(Js.Dict.t(string))
    | Fn(PM_Types.editorState => option(Js.Dict.t(string)));

  type js;

  let toJs =
    fun
    | Dict(dict) => Obj.magic(dict)
    | Fn(fn) => Obj.magic(fn);
};

module ScrollThreshold = {
  type t =
    | Int(int)
    | Pos(pos);

  type js;

  let toJs =
    fun
    | Int(i) => Obj.magic(i)
    | Pos(pos) => Obj.magic(pos);
};

module ScrollMargin = ScrollThreshold;

module Ext = {
  [@bs.deriving abstract]
  type t = {
    [@bs.optional]
    handleDOMEvents:
      Js.Dict.t((~view: Types.editorView, ~event: Dom.event) => bool),
    [@bs.optional]
    handleKeyDown:
      (~view: Types.editorView, ~event: Dom.keyboardEvent) => bool,
    [@bs.optional]
    handleKeyPress:
      (~view: Types.editorView, ~event: Dom.keyboardEvent) => bool,
    [@bs.optional]
    handleTextInput:
      (~view: Types.editorView, ~from: int, ~to_: int, ~text: string) => bool,
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
    handleClick:
      (~view: Types.editorView, ~pos: int, ~event: Dom.mouseEvent) => bool,
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
    handleDoubleClick:
      (~view: Types.editorView, ~pos: int, ~event: Dom.mouseEvent) => bool,
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
    handleTripleClick:
      (~view: Types.editorView, ~pos: int, ~event: Dom.mouseEvent) => bool,
    [@bs.optional]
    handlePaste:
      (~view: Types.editorView, ~event: Dom.event, ~slice: PM_Model.Slice.t) =>
      bool,
    [@bs.optional]
    handleDrop:
      (
        ~view: Types.editorView,
        ~event: Dom.event,
        ~slice: PM_Model.Slice.t,
        ~moved: bool
      ) =>
      bool,
    [@bs.optional]
    handleScrollToSelection: Types.editorView => bool,
    [@bs.optional]
    createSelectionBetween:
      (
        ~view: Types.editorView,
        ~anchor: PM_Model.ResolvedPos.t,
        ~head: PM_Model.ResolvedPos.t
      ) =>
      option(PM_Types.selection),
    [@bs.optional]
    domParser: PM_Model.DOMParser.t,
    [@bs.optional]
    transformPastedHTML: (~html: string) => string,
    [@bs.optional]
    clipboardParser: PM_Model.DOMParser.t,
    [@bs.optional]
    transformPastedText: (~text: string) => string,
    [@bs.optional]
    clipboardTextParser:
      (~text: string, ~context: PM_Model.ResolvedPos.t) => PM_Model.Slice.t,
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
    attributes: Attributes.js,
    [@bs.optional]
    scrollThreshold: ScrollThreshold.js,
    [@bs.optional]
    scrollMargin: ScrollMargin.js,
  };
};

type t = Ext.t;

let t =
    (
      ~handleDOMEvents:
         option(
           Js.Dict.t((~view: Types.editorView, ~event: Dom.event) => bool),
         )=?,
      ~handleKeyDown:
         option((~view: Types.editorView, ~event: Dom.keyboardEvent) => bool)=?,
      ~handleKeyPress:
         option((~view: Types.editorView, ~event: Dom.keyboardEvent) => bool)=?,
      ~handleTextInput:
         option(
           (~view: Types.editorView, ~from: int, ~to_: int, ~text: string) =>
           bool,
         )=?,
      ~handleClickOn:
         option(
           (
             ~view: Types.editorView,
             ~pos: int,
             ~node: PM_Model.Node.t,
             ~nodePos: int,
             ~event: Dom.mouseEvent,
             ~direct: bool
           ) =>
           bool,
         )=?,
      ~handleClick:
         option(
           (~view: Types.editorView, ~pos: int, ~event: Dom.mouseEvent) => bool,
         )=?,
      ~handleDoubleClickOn:
         option(
           (
             ~view: Types.editorView,
             ~pos: int,
             ~node: PM_Model.Node.t,
             ~nodePos: int,
             ~event: Dom.mouseEvent,
             ~direct: bool
           ) =>
           bool,
         )=?,
      ~handleDoubleClick:
         option(
           (~view: Types.editorView, ~pos: int, ~event: Dom.mouseEvent) => bool,
         )=?,
      ~handleTripleClickOn:
         option(
           (
             ~view: Types.editorView,
             ~pos: int,
             ~node: PM_Model.Node.t,
             ~nodePos: int,
             ~event: Dom.mouseEvent,
             ~direct: bool
           ) =>
           bool,
         )=?,
      ~handleTripleClick:
         option(
           (~view: Types.editorView, ~pos: int, ~event: Dom.mouseEvent) => bool,
         )=?,
      ~handlePaste:
         option(
           (
             ~view: Types.editorView,
             ~event: Dom.event,
             ~slice: PM_Model.Slice.t
           ) =>
           bool,
         )=?,
      ~handleDrop:
         option(
           (
             ~view: Types.editorView,
             ~event: Dom.event,
             ~slice: PM_Model.Slice.t,
             ~moved: bool
           ) =>
           bool,
         )=?,
      ~handleScrollToSelection: option(Types.editorView => bool)=?,
      ~createSelectionBetween:
         option(
           (
             ~view: Types.editorView,
             ~anchor: PM_Model.ResolvedPos.t,
             ~head: PM_Model.ResolvedPos.t
           ) =>
           option(PM_Types.selection),
         )=?,
      ~domParser: option(PM_Model.DOMParser.t)=?,
      ~transformPastedHTML: option((~html: string) => string)=?,
      ~clipboardParser: option(PM_Model.DOMParser.t)=?,
      ~transformPastedText: option((~text: string) => string)=?,
      ~clipboardTextParser:
         option(
           (~text: string, ~context: PM_Model.ResolvedPos.t) =>
           PM_Model.Slice.t,
         )=?,
      ~transformPasted: option(PM_Model.Slice.t => PM_Model.Slice.t)=?,
      ~nodeViews:
         option(
           Js.Dict.t(
             (
               ~node: PM_Model.Node.t,
               ~view: Types.editorView,
               ~getPos: unit => int,
               ~decorations: array(PM_Decoration.t)
             ) =>
             PM_NodeView.t,
           ),
         )=?,
      ~clipboardSerializer: option(PM_Model.DOMSerializer.t)=?,
      ~clipboardTextSerializer: option(PM_Model.Slice.t => string)=?,
      ~decorations: option(PM_Types.editorState => Types.decorationSet)=?,
      ~editable: option(PM_Types.editorState => bool)=?,
      ~attributes: option(Attributes.t)=?,
      ~scrollThreshold: option(ScrollThreshold.t)=?,
      ~scrollMargin: option(ScrollMargin.t)=?,
      (),
    ) =>
  Belt.(
    Ext.t(
      ~handleDOMEvents?,
      ~handleKeyDown?,
      ~handleKeyPress?,
      ~handleTextInput?,
      ~handleClickOn?,
      ~handleClick?,
      ~handleDoubleClickOn?,
      ~handleDoubleClick?,
      ~handleTripleClickOn?,
      ~handleTripleClick?,
      ~handlePaste?,
      ~handleDrop?,
      ~handleScrollToSelection?,
      ~createSelectionBetween?,
      ~domParser?,
      ~transformPastedHTML?,
      ~clipboardParser?,
      ~transformPastedText?,
      ~clipboardTextParser?,
      ~transformPasted?,
      ~nodeViews?,
      ~clipboardSerializer?,
      ~clipboardTextSerializer?,
      ~decorations?,
      ~editable?,
      ~attributes=?attributes->Option.map(Attributes.toJs),
      ~scrollThreshold=?scrollThreshold->Option.map(ScrollThreshold.toJs),
      ~scrollMargin=?scrollMargin->Option.map(ScrollMargin.toJs),
      (),
    )
  );
  