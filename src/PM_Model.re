module Types = PM_Types;
module Attrs = {
  type t;
  let make: Js.t({..}) => t = a => Obj.magic(a);
  let toJs: t => Js.t({..}) = a => Obj.magic(a);
  let empty = make(Js.Obj.empty());
};

module AttributeSpec = {
  type t;
  [@bs.obj] external make: (~default: 'a=?, unit) => t = "";
};

module DOMOutputSpec = {
  type spec =
    | LeafNode(string, Attrs.t): spec
    | Node(string, Attrs.t, spec): spec
    | Hole: spec;
  type t;
  let fromString: string => t = a => Obj.magic(a);
  let fromDomNode: Dom.node => t = a => Obj.magic(a);
  let fromSpec: spec => t =
    a => {
      let rec run: spec => t =
        a =>
          switch (a) {
          | LeafNode(str, obj) => (str, obj) |> Obj.magic
          | Hole => Obj.magic(0)
          | Node(str, obj, y) => (str, obj, run(y)) |> Obj.magic
          };
      run(a);
    };
};

module DOMSerializer = {
  type t;

  [@bs.module "prosemirror-model"] [@bs.new]
  external make:
    (
      ~nodes: Js.Dict.t(Types.node => DOMOutputSpec.t),
      ~marks: Js.Dict.t((Types.mark, ~inline: bool) => DOMOutputSpec.t)
    ) =>
    t =
    "DOMSerializer";

  [@bs.get] external nodes: t => Js.Dict.t(Types.node => DOMOutputSpec.t) = "";
  [@bs.get] external marks: t => Js.Dict.t((Types.mark, ~inline: bool) => DOMOutputSpec.t) = "";
  [@bs.send]
  external serializeFragment:
    (t, ~fragment: Types.fragment, ~options: Js.t({..})=?, unit) => Dom.documentFragment =
    "";
  [@bs.send]
  external serializeNode: (t, ~node: Types.node, ~options: Js.t({..})=?, unit) => Dom.node = "";

  [@bs.module "prosemirror-model"] [@bs.scope "DOMSerializer"]
  external renderSpec:
    (~doc: Dom.document, ~structure: DOMOutputSpec.t) =>
    {
      .
      "dom": Dom.node,
      "contentDOM": Js.Nullable.t(Dom.node),
    } =
    "";

  [@bs.module "prosemirror-model"] [@bs.scope "DOMSerializer"]
  external fromSpec: Types.schema => t = "";
};

module ParseRule = {
  [@bs.deriving abstract]
  type t = {
    [@bs.optional]
    tag: string,
    [@bs.optional]
    namespace: string,
    [@bs.optional]
    style: string,
    [@bs.optional]
    priority: int,
    [@bs.optional]
    context: string,
    [@bs.optional]
    node: string,
    [@bs.optional]
    mark: string,
    [@bs.optional]
    ignore: bool,
    [@bs.optional]
    skip: bool,
    [@bs.optional]
    attrs: Js.Dict.t(AttributeSpec.t),
    // This Js.t({.}) could be an object or false
    [@bs.optional] [@bs.as "getAttrs"]
    getAttrsWithNode: Dom.node => Attrs.t,
    [@bs.optional] [@bs.as "getAttrs"]
    getAttrsWithString: string => Attrs.t,
    [@bs.optional] [@bs.as "contentElement"]
    contentElementWithNode: Dom.node => Dom.node,
    [@bs.optional] [@bs.as "contentElement"]
    contentElementWithString: string => Dom.node,
    [@bs.optional]
    getContent: (Dom.node, Types.schema) => Types.fragment,
    // Only possible value is "full"
    [@bs.optional] [@bs.as "preserveWhitespace"]
    preserveWhitespaceFull: string,
    [@bs.optional]
    preserveWhitespace: bool,
  };
};

module Mark = {
  type t = Types.mark;
  [@bs.get] external type_: t => Types.markType = "type";
  [@bs.get] external attrs: t => Attrs.t = "";
  [@bs.send] external addToSet: (t, array(t)) => array(t) = "";
  [@bs.send] external removeFromSet: (t, array(t)) => array(t) = "";
  [@bs.send] external isInSet: (t, array(t)) => bool = "";
  [@bs.send] external eq: (t, t) => bool = "";
  [@bs.send] external toJSON: t => Js.Json.t = "";

  [@bs.module "prosemirror-model"] [@bs.scope "Mark"]
  external fromJSON: (~schema: Types.schema, ~json: Js.Json.t) => t = "";

  [@bs.module "prosemirror-model"] [@bs.scope "Mark"]
  external sameSet: (~a: array(t), ~b: array(t)) => bool = "";

  [@bs.module "prosemirror-model"] [@bs.scope "Mark"]
  external setFromArray: array(t) => array(t) = "setFrom";

  [@bs.module "prosemirror-model"] [@bs.scope "Mark"] external none: array(t) = "";
};

module Slice = {
  type t;

  [@bs.module "prosemirror-model"] [@bs.new]
  external make: (~content: Types.fragment, ~openStart: int, ~openEnd: int) => t = "Slice";

  [@bs.get] external content: t => Types.fragment = "";
  [@bs.get] external openStart: t => int = "";
  [@bs.get] external openEnd: t => int = "";
  [@bs.get] external size: t => int = "";
  [@bs.send] external eq: (t, t) => bool = "";
  [@bs.send] external toJSON: t => Js.Json.t = "";

  [@bs.module "prosemirror-model"] [@bs.scope "Slice"]
  external fromJSON: (~schema: Types.schema, ~json: Js.Json.t) => t = "";

  [@bs.module "prosemirror-model"] [@bs.scope "Slice"]
  external maxOpen: (~fragment: Types.fragment, ~openIsolating: bool=?) => t = "";

  [@bs.module "prosemirror-model"] [@bs.scope "Slice"] external empty: t = "";
};

module ContentMatch = {
  type t;
  [@bs.get] external validEnd: t => bool = "";
  [@bs.return nullable] [@bs.send] external matchType: (t, Types.nodeType) => option(t) = "";
  [@bs.return nullable] [@bs.send]
  external matchFragment:
    (t, ~frag: Types.fragment, ~start: int=?, ~end_: int=?, unit) => option(t) =
    "";
  [@bs.return nullable] [@bs.get] external defaultType: t => option(Types.nodeType) = "";
  [@bs.return nullable] [@bs.send]
  external fillBefore:
    (t, ~after: Types.fragment, ~toEnd: bool=?, ~startIndex: int=?, unit) =>
    option(Types.fragment) =
    "";
  [@bs.return nullable] [@bs.send]
  external findWrapping: (t, Types.nodeType) => option(array(Types.nodeType)) = "";
  [@bs.get] external edgeCount: t => int = "";
  [@bs.send]
  external edge:
    t =>
    {
      .
      "type_": Types.nodeType,
      "next": t,
    } =
    "";
};

module NodeRange = {
  type t = Types.nodeRange;

  [@bs.module "prosemirror-model"] [@bs.new]
  external make:
    (~resolvedFrom: Types.resolvedPos, ~resolvedTo: Types.resolvedPos, ~depth: int) => t =
    "NodeRange";

  [@bs.get] external resolvedFrom: t => Types.resolvedPos = "$from";
  [@bs.get] external resolvedTo: t => Types.resolvedPos = "$to";
  [@bs.get] external depth: t => int = "";
  [@bs.get] external start: t => int = "";
  [@bs.get] external end_: t => int = "end";
  [@bs.get] external parent: t => Types.node = "";
  [@bs.get] external startIndex: t => int = "";
  [@bs.get] external endIndex: t => int = "";
};

module NodeSpec = {
  [@bs.deriving abstract]
  type t = {
    [@bs.optional]
    content: string,
    [@bs.optional]
    marks: string,
    [@bs.optional]
    group: string,
    [@bs.optional]
    inline: bool,
    [@bs.optional]
    atom: bool,
    [@bs.optional]
    attrs: Js.Dict.t(AttributeSpec.t),
    [@bs.optional]
    selectable: bool,
    [@bs.optional]
    draggable: bool,
    [@bs.optional]
    code: bool,
    [@bs.optional]
    defining: bool,
    [@bs.optional]
    isolating: bool,
    [@bs.optional]
    toDOM: Types.node => DOMOutputSpec.t,
    [@bs.optional]
    parseDOM: array(ParseRule.t),
    [@bs.optional]
    toDebugString: Types.node => string,
  };
};

module NodeType = {
  type t = Types.nodeType;

  [@bs.get] external name: t => string = "";
  [@bs.get] external schema: t => Types.schema = "";
  [@bs.get] external spec: t => NodeSpec.t = "";
  [@bs.get] external contentMatch: t => ContentMatch.t = "";
  [@bs.get] external hasInlineContent: t => bool = "inlineContent";
  [@bs.get] external isBlock: t => bool = "";
  [@bs.get] external isText: t => bool = "";
  [@bs.get] external isInline: t => bool = "";
  [@bs.get] external isTextBlock: t => bool = "";
  [@bs.get] external isLeaf: t => bool = "";
  [@bs.get] external isAtom: t => bool = "";

  [@bs.send]
  external create:
    (
      t,
      ~attrs: Attrs.t=?,
      ~content: [@bs.unwrap] [
                  | `Fragment(Types.fragment)
                  | `Node(Types.node)
                  | `NodeArray(array(Types.node))
                ]
                  =?,
      ~marks: array(Types.mark)=?,
      unit
    ) =>
    Types.node =
    "";

  [@bs.send]
  external createCheckedExn:
    (
      t,
      ~attrs: Attrs.t=?,
      ~content: [@bs.unwrap] [
                  | `Fragment(Types.fragment)
                  | `Node(Types.node)
                  | `NodeArray(array(Types.node))
                ]
                  =?,
      ~marks: array(Types.mark)=?,
      unit
    ) =>
    Types.node =
    "createChecked";

  [@bs.return nullable] [@bs.send]
  external createAndFill:
    (
      t,
      ~attrs: Attrs.t=?,
      ~content: [@bs.unwrap] [
                  | `Fragment(Types.fragment)
                  | `Node(Types.node)
                  | `NodeArray(array(Types.node))
                ]
                  =?,
      ~marks: array(Types.mark)=?,
      unit
    ) =>
    option(Types.node) =
    "";

  [@bs.send] external validContent: (t, ~content: Types.fragment) => bool = "";
  [@bs.send] external allowsMarkType: (t, ~markType: Types.markType) => bool = "";
  [@bs.send] external allowsMarks: (t, ~marks: array(Types.mark)) => bool = "";
  [@bs.send] external allowedMarks: (t, ~marks: array(Types.mark)) => array(Types.mark) = "";
};

module MarkSpec = {
  [@bs.deriving abstract]
  type t = {
    [@bs.optional]
    group: string,
    [@bs.optional]
    attrs: Js.Dict.t(AttributeSpec.t),
    [@bs.optional]
    inclusive: bool,
    [@bs.optional]
    spanning: bool,
    [@bs.optional]
    excludes: string,
    [@bs.optional]
    toDOM: Types.node => DOMOutputSpec.t,
    [@bs.optional]
    parseDOM: array(ParseRule.t),
  };
};

module MarkType = {
  type t = Types.markType;
  [@bs.get] external name: t => string = "";
  [@bs.get] external schema: t => Types.schema = "";
  [@bs.get] external spec: t => MarkSpec.t = "";
  [@bs.send] external create: (t, ~attrs: Attrs.t=?, unit) => Types.mark = "";
  [@bs.send] external removeFromSet: (t, ~set: array(Types.mark)) => array(Types.mark) = "";
  [@bs.send] [@bs.return nullable]
  external isInSet: (t, array(Types.mark)) => option(Types.mark) = "";
  [@bs.send] external excludes: (t, ~other: t) => bool = "";
};

module SchemaSpec = {
  [@bs.deriving abstract]
  type t = {
    nodes: OrderedMap.t(NodeSpec.t),
    marks: OrderedMap.t(MarkSpec.t),
    [@bs.optional]
    topNode: string,
  };
};

module Schema = {
  type t;
  [@bs.get] external nodes: t => Js.Dict.t(Types.nodeType) = "";
  [@bs.get] external marks: t => Js.Dict.t(Types.markType) = "";
  [@bs.get] external spec: t => SchemaSpec.t = "";
  [@bs.get] external topNodeType: t => Types.nodeType = "";
  [@bs.get] external cached: t => Attrs.t = "";
  [@bs.send]
  external node:
    (
      t,
      ~type_: [@bs.unwrap] [ | `String(string) | `NodeType(Types.nodeType)],
      ~attrs: Attrs.t=?,
      ~content: [@bs.unwrap] [
                  | `Fragment(Types.fragment)
                  | `Node(Types.node)
                  | `NodeArray(array(Types.node))
                ]
                  =?,
      unit
    ) =>
    Types.node =
    "";
  [@bs.send]
  external text: (t, ~text: string, ~marks: array(Types.mark)=?, unit) => Types.node = "";
  [@bs.send]
  external mark:
    (
      t,
      ~type_: [@bs.unwrap] [ | `String(string) | `MarkType(Types.markType)],
      ~attrs: Attrs.t=?,
      unit
    ) =>
    Types.mark =
    "";
  [@bs.send] external nodeFromJSON: (t, Js.Json.t) => Types.node = "";
  [@bs.send] external markFromJSON: (t, Js.Json.t) => Types.mark = "";
  [@bs.module "prosemirror-model"] [@bs.new] external make: SchemaSpec.t => t = "Schema";
};

module Node = {
  type t = Types.node;
  [@bs.get] external type_: t => NodeType.t = "type";
  [@bs.get] external attrs: t => Attrs.t = "";
  [@bs.get] external content: t => Types.fragment = "";
  [@bs.get] external marks: t => array(Mark.t) = "";
  [@bs.return nullable] [@bs.get] external text: t => option(string) = "";
  [@bs.get] external nodeSize: t => int = "";
  [@bs.get] external childCount: t => int = "";
  [@bs.send] external childExn: (t, int) => t = "child";
  [@bs.return nullable] [@bs.send] external maybeChild: (t, int) => option(t) = "";
  [@bs.send] external forEach: (t, ~f: (~node: t, ~offset: int, ~index: int) => unit) => unit = "";
  [@bs.send]
  external nodesBetween:
    (
      t,
      ~from: int,
      ~to_: int,
      ~f: (~node: t, ~pos: int, ~parent: t, ~index: int) => bool,
      ~startPos: int=?,
      unit
    ) =>
    unit =
    "";
  [@bs.send] external descendants: (t, ~f: (~node: t, ~pos: int, ~parent: t) => bool) => unit = "";
  [@bs.get] external textContent: t => string = "";
  [@bs.send]
  external textBetween:
    (t, ~from: int, ~to_: int, ~blockSeparator: string=?, ~leafText: string=?, unit) => string =
    "";
  [@bs.return nullable] [@bs.get] external firstChild: t => option(t) = "";
  [@bs.return nullable] [@bs.get] external lastChild: t => option(t) = "";
  [@bs.send] external eq: (t, t) => bool = "";
  [@bs.send] external sameMarkUp: (t, t) => bool = "";
  [@bs.send]
  external hasMarkup:
    (t, ~type_: NodeType.t, ~attrs: Attrs.t=?, ~marks: array(Mark.t)=?, unit) => bool =
    "";
  [@bs.send] external copy: (t, ~content: Types.fragment=?, unit) => t = "";
  [@bs.send] external mark: (t, ~marks: array(Mark.t)) => t = "";
  [@bs.send] external cut: (t, ~from: int, ~to_: int=?, unit) => t = "";
  [@bs.send] external slice: (t, ~from: int, ~to_: int=?, unit) => Slice.t = "";
  [@bs.send] external replaceExn: (t, ~from: int, ~to_: int, ~slice: Slice.t) => t = "replace";
  [@bs.return nullable] [@bs.send] external nodeAt: (t, ~pos: int) => option(t) = "";
  [@bs.send]
  external childAfter:
    (t, ~pos: int) =>
    {
      .
      "node": Js.Nullable.t(t),
      "index": int,
      "offset": int,
    } =
    "";
  [@bs.send]
  external childBefore:
    (t, ~pos: int) =>
    {
      .
      "node": Js.Nullable.t(t),
      "index": int,
      "offset": int,
    } =
    "";
  [@bs.send] external resolve: (t, ~pos: int) => Types.resolvedPos = "";
  [@bs.send] external rangeHasMark: (t, ~from: int, ~to_: int, ~type_: MarkType.t) => bool = "";
  [@bs.get] external isBlock: t => bool = "";
  [@bs.get] external isTextblock: t => bool = "";
  [@bs.get] external inLineContent: t => bool = "";
  [@bs.get] external isInline: t => bool = "";
  [@bs.get] external isText: t => bool = "";
  [@bs.get] external isLeaf: t => bool = "";
  [@bs.get] external isAtom: t => bool = "";
  [@bs.send] external toString: t => string = "";
  [@bs.send] external contentMatchAt: (t, ~index: int) => ContentMatch.t = "";
  [@bs.send]
  external canReplace:
    (t, ~from: int, ~to_: int, ~replacement: Types.fragment, ~start: int=?, ~end_: int=?, unit) =>
    bool =
    "";
  [@bs.send]
  external canReplaceWith:
    (t, ~from: int, ~to_: int, ~type_: NodeType.t, ~marks: array(Mark.t)=?, unit) => bool =
    "";
  [@bs.send] external canAppend: (t, ~other: t) => bool = "";
  [@bs.send] external checkExn: t => bool = "check";
  [@bs.send] external toJSON: t => Js.Json.t = "";

  [@bs.module "prosemirror-model"] [@bs.scope "Node"]
  external fromJSON: (~schema: Schema.t, ~json: Js.Json.t) => t = "";
};

module ResolvedPos = {
  type t = Types.resolvedPos;
  [@bs.get] external pos: t => int = "";
  [@bs.get] external depth: t => int = "";
  [@bs.get] external parentOffset: t => int = "";
  [@bs.get] external parent: t => Types.node = "";
  [@bs.get] external doc: t => Types.node = "";
  [@bs.send] external node: (t, ~depth: int=?, unit) => Types.node = "";
  [@bs.send] external index: (t, ~depth: int=?, unit) => int = "";
  [@bs.send] external indexAfter: (t, ~depth: int=?, unit) => int = "";
  [@bs.send] external start: (t, ~depth: int=?, unit) => int = "";
  [@bs.send] external end_: t => int = "end";
  [@bs.send] external before: t => int = "";
  [@bs.send] external after: t => int = "";
  [@bs.get] external textOffset: t => int = "";
  [@bs.return nullable] [@bs.get] external nodeAfter: t => option(Types.node) = "";
  [@bs.return nullable] [@bs.get] external nodeBefore: t => option(Types.node) = "";
  [@bs.send] external marks: t => array(Types.mark) = "";
  [@bs.return nullable] [@bs.send]
  external marksAcross: (t, ~resolvedEnd: t) => option(array(Types.mark)) = "";
  [@bs.send] external sharedDepth: (t, ~pos: int) => int = "";
  [@bs.return nullable] [@bs.send]
  external blockRange:
    (t, ~other: t=?, ~pred: Types.node => bool=?, unit) => option(Types.nodeRange) =
    "";
  [@bs.send] external sameParent: (t, ~other: t) => bool = "";
  [@bs.send] external max: (t, ~other: t) => t = "";
  [@bs.send] external min: (t, ~other: t) => t = "";
};

module Fragment = {
  type t = Types.fragment;

  [@bs.get] external size: t => int = "";
  [@bs.send]
  external nodesBetween:
    (
      t,
      ~from: int,
      ~to_: int,
      ~f: (~node: Node.t, ~pos: int, ~parent: Node.t, ~index: int) => bool,
      ~startPos: int=?,
      unit
    ) =>
    unit =
    "";
  [@bs.send]
  external descendants: (t, ~f: (~node: Node.t, ~pos: int, ~parent: t) => bool) => unit = "";
  [@bs.send] external append: (t, ~other: t) => t = "";
  [@bs.send] external cut: (t, ~from: int, ~to_: int=?, unit) => t = "";
  [@bs.send] external replaceChild: (t, ~index: int, ~node: Node.t) => t = "";
  [@bs.send] external eq: (t, t) => bool = "";
  [@bs.return nullable] [@bs.get] external firstChild: t => option(Node.t) = "";
  [@bs.return nullable] [@bs.get] external lastChild: t => option(Node.t) = "";
  [@bs.get] external childCount: t => int = "";
  [@bs.send] external childExn: (t, int) => Node.t = "child";
  [@bs.return nullable] [@bs.send] external maybeChild: (t, int) => option(Node.t) = "";
  [@bs.send]
  external forEach: (t, ~f: (~node: Node.t, ~offset: int, ~index: int) => unit) => unit = "";
  [@bs.return nullable] [@bs.send] external findDiffStart: (t, ~other: t) => option(int) = "";
  [@bs.return nullable] [@bs.send]
  external findDiffEnd:
    (t, ~other: t) =>
    option({
      .
      "a": int,
      "b": int,
    }) =
    "";
  [@bs.send] external toString: t => string = "";
  [@bs.send] external toJSON: t => Js.Json.t = "";

  [@bs.module "prosemirror-model"] [@bs.scope "Fragment"]
  external fromJSON: (Schema.t, Js.Json.t) => t = "";

  [@bs.module "prosemirror-model"] [@bs.scope "Fragment"]
  external fromArray: array(Node.t) => t = "";

  [@bs.module "prosemirror-model"] [@bs.scope "Fragment"] external fromNode: Node.t => t = "from";
  [@bs.module "prosemirror-model"] [@bs.scope "Fragment"] external empty: t = "";
};

module ParseOptions = {
  [@bs.deriving abstract]
  type t = {
    // Only possible value is "full"
    [@bs.optional] [@bs.as "preserveWhitespace"]
    preserveWhitespaceFull: string,
    [@bs.optional]
    preserveWhitespace: bool,
    [@bs.optional]
    findPositions:
      array({
        .
        "node": Dom.node,
        "offset": int,
      }),
    [@bs.optional]
    from: int,
    [@bs.optional] [@bs.as "to"]
    to_: int,
    [@bs.optional]
    topNode: ContentMatch.t,
    [@bs.optional]
    context: Types.resolvedPos,
  };
};

module DOMParser = {
  type t;
  [@bs.module "prosemirror-model"] [@bs.scope "DOMParser"] [@bs.new]
  external make: (~schema: Schema.t, ~rules: array(ParseRule.t)) => t = "";

  [@bs.get] external schema: t => Schema.t = "";

  [@bs.get] external rules: t => array(ParseRule.t) = "";

  [@bs.send] external parse: (t, ~dom: Dom.node, ~options: ParseOptions.t=?, unit) => Node.t = "";

  [@bs.send]
  external parseSlice: (t, ~dom: Dom.node, ~options: ParseOptions.t=?, unit) => Slice.t = "";

  [@bs.module "prosemirror-model"] [@bs.scope "DOMParser"] external fromSchema: Schema.t => t = "";
};
