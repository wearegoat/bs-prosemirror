module Model = PM_Model;
module ResolvedPos = Model.ResolvedPos;
module Transform = PM_Transform;
module Types = PM_Types;
module EditorProps = PM_EditorProps;

module SelectionRange = {
  type t;
  [@bs.module "prosemirror-state"] [@bs.new]
  external make: (~resolvedFrom: Model.ResolvedPos.t, ~resolvedTo: Model.ResolvedPos.t) => t =
    "SelectionRange";

  [@bs.get] external resolvedFrom: t => Model.ResolvedPos.t = "$from";
  [@bs.get] external resolvedTo: t => Model.ResolvedPos.t = "$to";
};

module SelectionBookmark = {
  type t;
  [@bs.send] external map: (t, ~mapping: Transform.Mapping.t) => t = "";
  [@bs.send] external resolve: (t, ~doc: Model.Node.t) => Types.selection = "";
};

module SelectionKind = {
  type selectionClass;

  [@bs.module "prosemirror-state"] external nodeSelection: selectionClass = "NodeSelection";
  [@bs.module "prosemirror-state"] external textSelection: selectionClass = "TextSelection";
  [@bs.module "prosemirror-state"] external allSelection: selectionClass = "AllSelection";

  let isKind: (Types.selection, selectionClass) => bool = [%bs.raw
    {|
        function (selection, typ) {
          return selection instanceof typ
        }
      |}
  ];
  let selectionToNodeSelection = a =>
    if (isKind(a, nodeSelection)) {
      let ns: Types.nodeSelection = Obj.magic(a);
      Some(ns);
    } else {
      None;
    };
  let selectionToTextSelection = a =>
    if (isKind(a, textSelection)) {
      let ns: Types.textSelection = Obj.magic(a);
      Some(ns);
    } else {
      None;
    };
  let selectionToAllSelection = a =>
    if (isKind(a, allSelection)) {
      let ns: Types.allSelection = Obj.magic(a);
      Some(ns);
    } else {
      None;
    };
  let nodeSelectionToSelection: Types.nodeSelection => Types.selection = a => Obj.magic(a);
  let textSelectionToSelection: Types.textSelection => Types.selection = a => Obj.magic(a);
  let allSelectionToSelection: Types.allSelection => Types.selection = a => Obj.magic(a);
};

module Selection = {
  type t = Types.selection;
  module type T = {
    type t;
    let ranges: t => array(SelectionRange.t);
    let resolvedAnchor: t => Model.ResolvedPos.t;
    let resolvedHead: t => Model.ResolvedPos.t;
    let anchor: t => int;
    let head: t => int;
    let from: t => int;
    let to_: t => int;
    let resolvedFrom: t => Model.ResolvedPos.t;
    let resolvedTo: t => Model.ResolvedPos.t;
    let empty: t => bool;
    let eq: (t, t) => bool;
    let map: (t, ~doc: Model.Node.t, ~mapping: Transform.Mapping.t) => t;
    let content: t => Model.Slice.t;
    let replace: (t, ~tr: Types.transaction, ~content: Model.Slice.t=?, unit) => unit;
    let replaceWith: (t, ~tr: Types.transaction, ~node: Model.Node.t) => unit;
    let toJSON: t => Js.Json.t;
    let getBookmark: t => SelectionBookmark.t;
    let visible: t => bool;
    let findFrom:
      (~resolvedPos: Model.ResolvedPos.t, ~dir: int, ~textOnly: bool=?, unit) => option(t);
    let near: (~resolvedPos: Model.ResolvedPos.t, ~bias: int=?, unit) => option(t);
    let atStart: (~doc: Model.Node.t) => t;
    let atEnd: (~doc: Model.Node.t) => t;
    let fromJSON: (~doc: Model.Node.t, ~json: Js.Json.t) => t;
    let jsonID: (~id: string, ~selectionClass: t) => t;
  };
  module Make = (M: {type t;}) : (T with type t := M.t) => {
    type t = M.t;
    [@bs.get] external ranges: t => array(SelectionRange.t) = "";
    [@bs.get] external resolvedAnchor: t => Model.ResolvedPos.t = "$anchor";
    [@bs.get] external resolvedHead: t => Model.ResolvedPos.t = "$head";
    [@bs.get] external anchor: t => int = "";
    [@bs.get] external head: t => int = "";
    [@bs.get] external from: t => int = "";
    [@bs.get] external to_: t => int = "to";
    [@bs.get] external resolvedFrom: t => Model.ResolvedPos.t = "$from";
    [@bs.get] external resolvedTo: t => Model.ResolvedPos.t = "$to";
    [@bs.get] external empty: t => bool = "";
    [@bs.send] external eq: (t, t) => bool = "";
    [@bs.send] external map: (t, ~doc: Model.Node.t, ~mapping: Transform.Mapping.t) => t = "";
    [@bs.send] external content: t => Model.Slice.t = "";
    [@bs.send]
    external replace: (t, ~tr: Types.transaction, ~content: Model.Slice.t=?, unit) => unit = "";
    [@bs.send] external replaceWith: (t, ~tr: Types.transaction, ~node: Model.Node.t) => unit = "";
    [@bs.send] external toJSON: t => Js.Json.t = "";
    [@bs.send] external getBookmark: t => SelectionBookmark.t = "";
    [@bs.get] external visible: t => bool = "";

    [@bs.return nullable] [@bs.module "prosemirror-state"] [@bs.scope "Selection"]
    external findFrom:
      (~resolvedPos: Model.ResolvedPos.t, ~dir: int, ~textOnly: bool=?, unit) => option(t) =
      "";

    [@bs.return nullable] [@bs.module "prosemirror-state"] [@bs.scope "Selection"]
    external near: (~resolvedPos: Model.ResolvedPos.t, ~bias: int=?, unit) => option(t) = "";

    [@bs.module "prosemirror-state"] [@bs.scope "Selection"]
    external atStart: (~doc: Model.Node.t) => t = "";

    [@bs.module "prosemirror-state"] [@bs.scope "Selection"]
    external atEnd: (~doc: Model.Node.t) => t = "";

    [@bs.module "prosemirror-state"] [@bs.scope "Selection"]
    external fromJSON: (~doc: Model.Node.t, ~json: Js.Json.t) => t = "";

    /* REMINDER -> The type of stepClass is constructor<Step> however I am not sure how to model that
        Alexey: I checked a couple of "wrappers" for prosemirror. At least at this point I couldn't
        find any usage of this function anywhere. static jsonID(id: string, selectionClass:
        constructor<Selection>)
        To be able to deserialize selections from JSON, custom selection classes must register
        themselves with an ID string, so that they can be disambiguated.
        Try to pick something that's unlikely to clash with classes from other modules.
       */
    [@bs.module "prosemirror-state"] [@bs.scope "Selection"]
    external jsonID: (~id: string, ~selectionClass: t) => t = "";
  };
  include Make({
    type nonrec t = t;
  });

  [@bs.module "prosemirror-state"] [@bs.new]
  external make:
    (
      ~resolvedAnchor: Model.ResolvedPos.t,
      ~resolvedHead: Model.ResolvedPos.t,
      ~ranges: array(SelectionRange.t)=?,
      unit
    ) =>
    t =
    "Selection";
  let toNodeSelection = SelectionKind.selectionToNodeSelection;
  let toTextSelection = SelectionKind.selectionToTextSelection;
  let toAllSelection = SelectionKind.selectionToAllSelection;
  let fromNodeSelection = SelectionKind.nodeSelectionToSelection;
  let fromTextSelection = SelectionKind.textSelectionToSelection;
  let fromAllSelection = SelectionKind.allSelectionToSelection;
};

module TextSelection = {
  type t = Types.textSelection;

  include Selection.Make({
    type nonrec t = t;
  });

  [@bs.send] external resolvedCursor: t => Model.ResolvedPos.t = "$cursor";

  [@bs.module "prosemirror-state"] [@bs.new]
  external make:
    (~resolvedAnchor: Model.ResolvedPos.t, ~resolvedHead: Model.ResolvedPos.t=?, unit) => t =
    "TextSelection";

  [@bs.module "prosemirror-state"] [@bs.scope "TextSelection"]
  external create: (~doc: Model.Node.t, ~anchor: int, ~head: int=?, unit) => t = "";

  [@bs.module "prosemirror-state"] [@bs.scope "TextSelection"]
  external between:
    (
      ~resolvedAnchor: Model.ResolvedPos.t,
      ~resolvedHead: Model.ResolvedPos.t,
      ~bias: int=?,
      unit
    ) =>
    Selection.t =
    "";
  let fromSelection = SelectionKind.selectionToTextSelection;
};

module NodeSelection = {
  type t = Types.nodeSelection;
  include Selection.Make({
    type nonrec t = t;
  });
  [@bs.module "prosemirror-state"] [@bs.new]
  external make:
    (~resolvedAnchor: Model.ResolvedPos.t, ~resolvedHead: Model.ResolvedPos.t, unit) => t =
    "NodeSelection";

  [@bs.get] external node: t => Model.Node.t = "";

  [@bs.module "prosemirror-state"] [@bs.scope "NodeSelection"]
  external create: (~doc: Model.Node.t, ~from: int) => t = "";

  [@bs.module "prosemirror-state"] [@bs.scope "NodeSelection"]
  external isSelectable: (~node: Model.Node.t) => bool = "";

  let fromSelection = SelectionKind.selectionToNodeSelection;
};

module AllSelection = {
  type t = Types.allSelection;
  include Selection.Make({
    type nonrec t = t;
  });
  [@bs.module "prosemirror-state"] [@bs.new]
  external make: (~doc: Model.Node.t) => t = "AllSelection";
  let fromSelection = SelectionKind.selectionToAllSelection;
};

module EditorState = {
  type t = Types.editorState;

  module Config = {
    [@bs.deriving abstract]
    type t = {
      [@bs.optional]
      schema: Model.Schema.t,
      [@bs.optional]
      doc: Model.Node.t,
      [@bs.optional]
      selection: Selection.t,
      [@bs.optional]
      storedMarks: array(Model.Mark.t),
      [@bs.optional]
      plugins: array(Types.plugin),
    };
    let make = t;
  };

  [@bs.get] external doc: t => Model.Node.t = "";
  [@bs.get] external selection: t => Selection.t = "";
  [@bs.return nullable] [@bs.get] external storedMarks: t => option(array(Model.Mark.t)) = "";
  [@bs.get] external schema: t => Model.Schema.t = "";
  [@bs.get] external plugins: t => array(Types.plugin) = "";
  [@bs.send] external apply: (t, Types.transaction) => t = "";
  [@bs.send]
  external applyTransaction:
    (t, Types.transaction) =>
    {
      .
      "state": t,
      "transactions": array(Types.transaction),
    } =
    "";
  [@bs.get] external tr: t => Types.transaction = "";
  [@bs.send] external reconfigure: (t, Config.t) => t = "";
  [@bs.send]
  external toJSON:
    (
      t,
      ~pluginFields: [@bs.unwrap] [
                       | `Plugins(Js.Dict.t(Types.plugin))
                       | `String(string)
                       | `Int(int)
                     ]
                       =?,
      unit
    ) =>
    Js.Json.t =
    "";
  [@bs.module "prosemirror-state"] [@bs.scope "EditorState"] external create: Config.t => t = "";
  [@bs.module "prosemirror-state"] [@bs.scope "EditorState"]
  external fromJSON:
    (~config: Config.t, ~json: Js.Json.t, ~pluginFields: Js.Dict.t(Types.plugin)=?, unit) => t =
    "";
  /* Apply the given transaction to produce a new state. */
};

module PluginKey = {
  type t;

  [@bs.module "prosemirror-state"] [@bs.new]
  external make: (~name: string=?, unit) => t = "PluginKey";

  [@bs.return nullable] [@bs.send]
  external get: (t, Types.editorState) => option(Types.plugin) = "";

  [@bs.send] external getState: (t, Types.editorState) => option(Js.t({..})) = "";
};

module StateField = {
  [@bs.deriving abstract]
  type t = {
    init: (~config: EditorState.Config.t, ~instance: Types.editorState) => Types.editorState,
    apply:
      (
        ~tr: Types.transaction,
        ~value: t,
        ~oldState: Types.editorState,
        ~newState: Types.editorState
      ) =>
      Types.editorState,
    [@bs.optional]
    toJSON: (~value: t) => Js.Json.t,
    [@bs.optional]
    fromJSON: (~config: EditorState.Config.t, ~value: t, ~state: Types.editorState) => t,
  };
  let make = t;
};

module PluginSpec = {
  module View = {
    type update = (Types.editorView, Types.editorState) => unit;
    type destroy = unit => unit;
    [@bs.deriving abstract]
    type t = {
      [@bs.optional]
      update,
      [@bs.optional]
      destroy,
    };
    let make = t;
  };

  [@bs.deriving abstract]
  type t = {
    [@bs.optional]
    props: EditorProps.t,
    [@bs.optional]
    state: StateField.t,
    [@bs.optional]
    key: PluginKey.t,
    [@bs.optional]
    view: Types.editorView => View.t,
    [@bs.optional]
    filterTransaction: (Types.transaction, Types.editorState) => bool,
    [@bs.optional]
    appendTransaction:
      (
        ~transanctions: array(Types.transaction),
        ~oldState: Types.editorState,
        ~newState: Types.editorState
      ) =>
      Js.Nullable.t(Types.transaction),
  };

  let make = t;
};

module Plugin = {
  type t = Types.plugin;

  [@bs.module "prosemirror-state"] [@bs.new] external make: (~spec: PluginSpec.t) => t = "Plugin";

  [@bs.get] external props: t => EditorProps.t = "";
  [@bs.get] external spec: t => PluginSpec.t = "";
  [@bs.send] external getState: (t, ~state: Types.editorState) => Js.t({..}) = "";
};

module Transaction = {
  type t = Types.transaction;
  include PM_Transform.Transform.Make({
    type nonrec t = t;
  });
  [@bs.get] external time: t => int = "";
  [@bs.return nullable] [@bs.get] external storedMarks: t => option(array(Model.Mark.t)) = "";
  [@bs.get] external selection: t => Selection.t = "";
  [@bs.send] external setSelection: (t, Selection.t) => t = "";
  [@bs.get] external selectionSet: t => bool = "";
  [@bs.send] external setStoredMarks: (t, ~marks: array(Model.Mark.t)=?, unit) => t = "";
  [@bs.send] external addStoredMark: (t, ~mark: Model.Mark.t) => t = "";
  [@bs.send] external ensureMarks: (t, array(PM_Model.Mark.t)) => t = "";
  [@bs.send]
  external removeStoredMark:
    (t, ~markOrMarkType: [@bs.unwrap] [ | `Mark(Model.Mark.t) | `MarkType(Model.MarkType.t)]) =>
    t =
    "";
  [@bs.get] external storedMarksSet: t => bool = "";
  [@bs.send] external setTime: (t, int) => t = "";
  [@bs.send] external replaceSelection: (t, Model.Slice.t) => t = "";
  [@bs.send]
  external replaceSelectionWith: (t, ~node: Model.Node.t, ~inheritMarks: bool=?, unit) => t = "";
  [@bs.send] external deleteSelection: t => t = "";
  [@bs.send] external insertText: (t, ~test: string, ~from: int=?, ~to_: int=?, unit) => t = "";
  [@bs.send]
  external setMeta:
    (
      t,
      ~key: [@bs.unwrap] [ | `String(string) | `Plugin(Plugin.t) | `PluginKey(PluginKey.t)],
      ~value: Js.t({..})
    ) =>
    t =
    "";
  [@bs.send]
  external getMeta:
    (
      t,
      ~key: [@bs.unwrap] [ | `String(string) | `Plugin(Plugin.t) | `PluginKey(PluginKey.t)]
    ) =>
    Js.t({..}) =
    "";
  [@bs.get] external isGeneric: t => bool = "";
  [@bs.send] external scrollIntoView: t => t = "";
};
