module Types = PM_Types;

module EditorProps = PM_EditorProps;

module Decoration = PM_Decoration;

module NodeView = PM_NodeView;

type t = PM_Types.editorView;

module DecorationSet = {
  type t;

  [@bs.module "prosemirror-view"] [@bs.scope "DecorationSet"]
  external create: (~doc: PM_Model.Node.t, ~decorations: array(Decoration.t)) => t = "create";

  [@bs.module "prosemirror-view"] [@bs.scope "DecorationSet"] external empty: t = "empty";

  [@bs.send]
  external find:
    (t, ~start: int=?, ~end_: int=?, ~predicate: Decoration.spec => bool=?, unit) =>
    array(Decoration.t) =
    "find";

  [@bs.send]
  external map:
    (
      t,
      ~mapping: PM_Transform.Mapping.t,
      ~doc: PM_Model.Node.t,
      ~option: {. "onRemove": Decoration.spec => unit}=?,
      unit
    ) =>
    t =
    "map";

  [@bs.send]
  external add: (t, ~doc: PM_Model.Node.t, ~decorations: array(Decoration.t)) => t = "add";

  [@bs.send] external remove: (t, ~decorations: array(Decoration.t)) => t = "remove";
};

module DirectEditorProps = {
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
      option(PM_State.Selection.t),
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
          ~decorations: array(Decoration.t)
        ) =>
        NodeView.t,
      ),
    [@bs.optional]
    clipboardSerializer: PM_Model.DOMSerializer.t,
    [@bs.optional]
    clipboardTextSerializer: PM_Model.Slice.t => string,
    [@bs.optional]
    decorations: PM_State.EditorState.t => DecorationSet.t,
    [@bs.optional]
    editable: PM_State.EditorState.t => bool,
    [@bs.optional]
    attributes: PM_Model.Attrs.t,
    [@bs.optional] [@bs.as "attributes"]
    attributesFn: PM_State.EditorState.t => Js.Nullable.t(PM_Model.Attrs.t),
    [@bs.optional]
    scrollThreshold: int,
    [@bs.optional] [@bs.as "scrollThreshold"]
    scrollThresholdObj: pos,
    [@bs.optional]
    scrollMargin: int,
    [@bs.optional] [@bs.as "scrollMarginObj"]
    scrollMarginObj: pos,
    state: PM_State.EditorState.t,
    [@bs.optional]
    dispatchTransaction: PM_State.Transaction.t => unit,
  };
};

[@bs.module "prosemirror-view"] [@bs.new]
external make:
  ([@bs.unwrap] [ | `Node(Dom.element) | `Fn(Dom.node => unit)], DirectEditorProps.t) => t =
  "EditorView";

[@bs.module "prosemirror-view"] [@bs.new]
external makeWithNullExt: (Js.Nullable.t(unit), DirectEditorProps.t) => t = "EditorView";

let makeWithNull = makeWithNullExt(Js.Nullable.null);

[@bs.get] external state: t => PM_State.EditorState.t = "state";

[@bs.get] external dom: t => Dom.element = "dom";

[@bs.send] [@bs.return nullable]
external dragging:
  t =>
  option({
    .
    "slice": PM_Model.Slice.t,
    "move": bool,
  }) =
  "dragging";

[@bs.send] external props: t => DirectEditorProps.t = "props";

[@bs.send] external update: (t, DirectEditorProps.t) => unit = "update";

[@bs.send] external setProps: (t, DirectEditorProps.t) => unit = "setProps";

/* TODO: A better solution for props should be found */
[@bs.send] external someProp: (t, ~propName: string, ~f: 'a => 'a) => unit = "someProp";

[@bs.send] external hasFocus: t => bool = "hasFocus";

[@bs.send] external focus: t => unit = "focus";

[@bs.get] external root: t => Dom.document = "root";

[@bs.send] [@bs.return nullable]
external posAtCoords:
  (
    t,
    {
      .
      "left": int,
      "top": int,
    }
  ) =>
  option({
    .
    "pos": int,
    "inside": int,
  }) =
  "posAtCoords";

[@bs.send]
external coordsAtPos:
  (t, int) =>
  {
    .
    "left": int,
    "right": int,
    "top": int,
    "bottom": int,
  } =
  "coordsAtPos";

[@bs.send]
external domAtPos:
  (t, int) =>
  {
    .
    "node": Dom.node,
    "offset": int,
  } =
  "domAtPos";

[@bs.send] [@bs.return nullable] external nodeDOM: (t, int) => option(Dom.node) = "nodeDOM";

[@bs.send]
external posAtDOM: (t, ~node: Dom.element, ~offset: int, ~bias: int=?, unit) => int = "posAtDOM";

[@bs.send]
external endOfTextblock:
  (
    t,
    ~dir: [@bs.string] [ | `up | `down | `left | `right | `forward | `backward],
    ~state: PM_State.EditorState.t=?,
    unit
  ) =>
  bool =
  "endOfTextblock";

/* Update the editor's state prop, without touching any of the other props. */
[@bs.send] external updateState: (t, PM_State.EditorState.t) => unit = "updateState";

/* Removes the editor from the DOM and destroys all node views. */
[@bs.send] external destroy: t => unit = "destroy";

[@bs.send] external dispatch: (t, PM_State.Transaction.t) => bool = "dispatch";
