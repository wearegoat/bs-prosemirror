module Model = PM_Model;
module Types = PM_Types;

module MapResult = {
  type t;
  [@bs.get] external pos: t => int = "pos";
  [@bs.get] external deleted: t => bool = "deleted";
};

module type MAPPABLE = {
  type t;
  /**
      Map a position through this object. When given, assoc (should be -1 or 1, defaults to 1)
      determines with which side the position is associated, which determines in which direction to move
      when a chunk of content is inserted at the mapped position.
      map(pos: number, assoc: ?⁠number) → number
      */
  let map: (t, ~pos: int, ~assoc: int=?, unit) => int;
  /**
      Map a position, and return an object containing additional information about the mapping. The
      result's deleted field tells you whether the position was deleted (completely enclosed in a
      replaced range) during the mapping. When content on only one side is deleted, the position itself
      is only considered deleted when assoc points in the direction of the deleted content.
      mapResult(pos: number, assoc: ?⁠number) → MapResult
      */
  let mapResult: (t, ~pos: int, ~assoc: int=?, unit) => MapResult.t;
};

module Mappable = {
  module Make = (M: {type t;}) => {
    type t = M.t;
    [@bs.send] external map: (t, ~pos: int, ~assoc: int=?, unit) => int = "map";
    [@bs.send]
    external mapResult: (t, ~pos: int, ~assoc: int=?, unit) => MapResult.t = "mapResult";
  };
};

module StepMap = {
  include Mappable.Make({
    type t;
  });

  [@bs.module "prosemirror-transform"] [@bs.new]
  external make: (~ranges: array(int)) => t = "StepMap";

  [@bs.send] external map: (t, ~pos: int, ~assoc: int=?, unit) => int = "map";
  [@bs.send] external mapResult: (t, ~pos: int, ~assoc: int=?, unit) => MapResult.t = "mapResult";

  [@bs.send]
  external forEach:
    (t, ~fn: (~oldStart: int, ~oldEnd: int, ~newStart: int, ~newEnd: int) => unit) => unit =
    "forEach";
  [@bs.send] external invert: t => unit = "invert";

  [@bs.module "prosemirror-transform"] [@bs.scope "StepMap"]
  external offset: (~n: int) => t = "offset";
};

module StepResult = {
  type t;
  [@bs.get] external doc: t => option(Types.node) = "doc";
  [@bs.return nullable] [@bs.get] external failed: t => option(string) = "failed";
  [@bs.module "prosemirror-transform"] [@bs.scope "StepResult"]
  external ok: (~doc: Types.node) => t = "ok";
  [@bs.module "prosemirror-transform"] [@bs.scope "StepResult"]
  external fail: (~message: string) => t = "fail";
  [@bs.module "prosemirror-transform"] [@bs.scope "StepResult"]
  external fromReplace: (~doc: Types.node, ~from: int, ~to_: int, ~slice: Model.Slice.t) => t =
    "fromReplace";
};

module Mapping = {
  include Mappable.Make({
    type t;
  });

  [@bs.module "prosemirror-transform"] [@bs.new]
  external make: (~maps: array(StepMap.t)=?, unit) => t = "Mapping";

  [@bs.send] external map: (t, ~pos: int, ~assoc: int=?, unit) => int = "map";
  [@bs.send] external mapResult: (t, ~pos: int, ~assoc: int=?, unit) => MapResult.t = "mapResult";
  [@bs.get] external maps: t => array(StepMap.t) = "maps";
  [@bs.get] external from: t => int = "from";
  [@bs.get] external to_: t => int = "to";
  [@bs.send] external slice: (t, ~from: int=?, ~to_: int=?, unit) => t = "slice";
  [@bs.send] external appendMap: (t, ~map: StepMap.t, ~mirrors: int=?, unit) => unit = "appendMap";
  [@bs.send] external appendMapping: (t, ~mapping: t) => unit = "appendMapping";
  [@bs.return nullable] [@bs.send]
  external getMirror: (t, ~number: int) => option(int) = "getMirror";
  [@bs.send] external appendMappingInverted: (t, ~mapping: t) => unit = "appendMappingInverted";
};

module Step = {
  type t;
  [@bs.send] external apply: (t, ~doc: Types.node) => StepResult.t = "apply";
  [@bs.send] external getMap: t => StepMap.t = "getMap";
  [@bs.send] external invert: (t, ~doc: Types.node) => t = "invert";
  [@bs.return nullable] [@bs.send] external map: (t, ~mapping: Mapping.t) => option(t) = "map";
  [@bs.return nullable] [@bs.send] external merge: (t, ~other: t) => option(t) = "merge";
  [@bs.send] external toJSON: t => Js.Json.t = "toJSON";
  [@bs.module "prosemirror-transform"] [@bs.scope "Step"]
  external fromJSON: (Model.Schema.t, Js.Json.t) => t = "fromJSON";
  /* REMINDER -> The type of stepClass is constructor<Step> however I am not sure how to model that

      static jsonID(id: string, selectionClass: constructor<Selection>)
      To be able to deserialize selections from JSON, custom selection classes must register
      themselves with an ID string, so that they can be disambiguated.
      Try to pick something that's unlikely to clash with classes from other modules.
     */
  [@bs.module "prosemirror-transform"] [@bs.scope "Step"]
  external jsonID: (~id: string, ~stepClass: t) => t = "jsonID";
};

module AddMarkStep = {
  include Step;
  [@bs.module "prosemirror-transform"] [@bs.new]
  external make: (~from: int, ~to_: int, ~mark: Model.Mark.t) => t = "AddMarkStep";
};

module RemoveMarkStep = {
  include Step;
  [@bs.module "prosemirror-transform"] [@bs.new]
  external make: (~from: int, ~to_: int, ~mark: Model.Mark.t) => t = "RemoveMarkStep";
};

module ReplaceStep = {
  include Step;
  [@bs.module "prosemirror-transform"] [@bs.new]
  external make: (~from: int, ~to_: int, ~slice: Model.Slice.t, ~structure: bool=?, unit) => t =
    "ReplaceStep";
};

module ReplaceAroundStep = {
  include Step;
  [@bs.module "prosemirror-transform"] [@bs.new]
  external make:
    (
      ~from: int,
      ~to_: int,
      ~gapFrom: int,
      ~gapTo: int,
      ~slice: Model.Slice.t,
      ~insert: int,
      ~structure: bool=?,
      unit
    ) =>
    t =
    "ReplaceAroundStep";
};

module Transform = {
  module type T = {
    type t;
    let make: (~doc: Model.Node.t) => t;
    let doc: t => Model.Node.t;
    let steps: t => array(Step.t);
    let docs: t => array(Model.Node.t);
    let mapping: t => Mapping.t;
    let before: t => Model.Node.t;
    let step: (t, ~step: Step.t) => t;
    let maybeStep: (t, ~step: Step.t) => StepResult.t;
    let docChanged: t => bool;
    let addMark: (t, ~from: int, ~to_: int, ~mark: Model.Mark.t) => t;
    let removeMark:
      (
        t,
        ~from: int,
        ~to_: int,
        ~mark: [ | `Mark(Model.Mark.t) | `MarkType(Model.MarkType.t)]=?,
        unit
      ) =>
      t;

    let clearIncompatible:
      (t, ~pos: int, ~parentType: Model.NodeType.t, ~match: Model.ContentMatch.t=?, unit) => t;

    let replace: (t, ~from: int, ~to_: int=?, ~slice: Model.Slice.t=?, unit) => t;

    let replaceWith:
      (
        t,
        ~from: int,
        ~to_: int,
        ~content: [
                    | `Fragment(Model.Fragment.t)
                    | `Node(Model.Node.t)
                    | `NodeArr(array(Model.Node.t))
                  ]
      ) =>
      t;

    let delete: (t, ~from: int, ~to_: int) => t;
    let insert:
      (
        t,
        ~pos: int,
        ~content: [
                    | `Fragment(Model.Fragment.t)
                    | `Node(Model.Node.t)
                    | `NodeArr(array(Model.Node.t))
                  ]
      ) =>
      t;

    let replaceRange: (t, ~from: int, ~to_: int, ~slice: Model.Slice.t) => t;
    let replaceRangeWith: (t, ~from: int, ~to_: int, ~node: Model.Node.t) => t;
    let deleteRange: (t, ~from: int, ~to_: int) => t;
    let lift: (t, ~range: Model.NodeRange.t, ~target: int) => t;
    let wrap:
      (
        t,
        ~range: Model.NodeRange.t,
        ~wrappers: array({
                     .
                     "_type": Model.NodeType.t,
                     "attrs": Model.Attrs.t,
                   })
      ) =>
      t;
    let setBlockType:
      (t, ~from: int, ~to_: int=?, ~type_: Model.NodeType.t, ~attrs: Model.Attrs.t=?, unit) => t;
    let setNodeMarkup:
      (
        t,
        ~pos: int,
        ~type_: Model.NodeType.t=?,
        ~attrs: Model.Attrs.t=?,
        ~marks: array(Model.Mark.t)=?,
        unit
      ) =>
      t;
    let split:
      (
        t,
        ~pos: int,
        ~depth: int=?,
        ~typesAfter: array({
                       .
                       "_type": Model.NodeType.t,
                       "attrs": Model.Attrs.t,
                     })
                       =?,
        unit
      ) =>
      t;
    let join: (t, ~pos: int, ~depth: int=?, unit) => t;

    /**
    The following helper functions can be useful when creating transformations or determining whether they are even
possible.
    */
    let replaceStep:
      (~doc: Model.Node.t, ~from: int, ~to_: int=?, ~slice: Model.Slice.t=?, unit) =>
      option(Step.t);
    let liftTarget: (~range: Model.NodeRange.t) => option(int);

    let findWrapping:
      (
        ~range: Model.NodeRange.t,
        ~nodeType: Model.NodeType.t,
        ~attrs: Model.Attrs.t=?,
        ~innerRange: Model.NodeRange.t=?,
        unit
      ) =>
      option(
        array({
          .
          "_type": Model.NodeType.t,
          "attrs": Model.Attrs.t,
        }),
      );
    let canSplit:
      (
        ~doc: Model.Node.t,
        ~pos: int,
        ~depth: int=?,
        ~typesAfter: array({
                       .
                       "_type": Model.NodeType.t,
                       "attrs": Model.Attrs.t,
                     })
                       =?,
        unit
      ) =>
      bool;
    let canJoin: (~doc: Model.Node.t, ~pos: int) => bool;
    let joinPoint: (~doc: Model.Node.t, ~pos: int, ~dir: int=?, unit) => option(int);
    let insertPoint: (~doc: Model.Node.t, ~pos: int, ~nodeType: Model.NodeType.t) => option(int);
    let dropPoint: (~doc: Model.Node.t, ~pos: int, ~silce: Model.Slice.t) => option(int);
  };

  module Make = (M: {type t;}) : (T with type t := M.t) => {
    type t = M.t;

    [@bs.module "prosemirror-transform"] [@bs.new]
    external make: (~doc: Model.Node.t) => t = "Transform";

    [@bs.get] external doc: t => Model.Node.t = "doc";
    [@bs.get] external steps: t => array(Step.t) = "steps";
    [@bs.get] external docs: t => array(Model.Node.t) = "docs";
    [@bs.get] external mapping: t => Mapping.t = "mapping";
    [@bs.get] external before: t => Model.Node.t = "before";
    [@bs.send] external step: (t, ~step: Step.t) => t = "step";
    [@bs.send] external maybeStep: (t, ~step: Step.t) => StepResult.t = "maybeStep";
    [@bs.get] external docChanged: t => bool = "docChanged";
    [@bs.send] external addMark: (t, ~from: int, ~to_: int, ~mark: Model.Mark.t) => t = "addMark";
    [@bs.send]
    external removeMark:
      (
        t,
        ~from: int,
        ~to_: int,
        ~mark: [@bs.unwrap] [ | `Mark(Model.Mark.t) | `MarkType(Model.MarkType.t)]=?,
        unit
      ) =>
      t =
      "removeMark";
    [@bs.send]
    external clearIncompatible:
      (t, ~pos: int, ~parentType: Model.NodeType.t, ~match: Model.ContentMatch.t=?, unit) => t =
      "clearIncompatible";
    [@bs.send]
    external replace: (t, ~from: int, ~to_: int=?, ~slice: Model.Slice.t=?, unit) => t = "replace";

    [@bs.send]
    external replaceWith:
      (
        t,
        ~from: int,
        ~to_: int,
        ~content: [@bs.unwrap] [
                    | `Fragment(Model.Fragment.t)
                    | `Node(Model.Node.t)
                    | `NodeArr(array(Model.Node.t))
                  ]
      ) =>
      t =
      "replaceWith";
    [@bs.send] external delete: (t, ~from: int, ~to_: int) => t = "delete";
    [@bs.send]
    external insert:
      (
        t,
        ~pos: int,
        ~content: [@bs.unwrap] [
                    | `Fragment(Model.Fragment.t)
                    | `Node(Model.Node.t)
                    | `NodeArr(array(Model.Node.t))
                  ]
      ) =>
      t =
      "insert";
    [@bs.send]
    external replaceRange: (t, ~from: int, ~to_: int, ~slice: Model.Slice.t) => t = "replaceRange";
    [@bs.send]
    external replaceRangeWith: (t, ~from: int, ~to_: int, ~node: Model.Node.t) => t =
      "replaceRangeWith";
    [@bs.send] external deleteRange: (t, ~from: int, ~to_: int) => t = "deleteRange";
    [@bs.send] external lift: (t, ~range: Model.NodeRange.t, ~target: int) => t = "lift";
    [@bs.send]
    external wrap:
      (
        t,
        ~range: Model.NodeRange.t,
        ~wrappers: array({
                     .
                     "_type": Model.NodeType.t,
                     "attrs": Model.Attrs.t,
                   })
      ) =>
      t =
      "wrap";
    [@bs.send]
    external setBlockType:
      (t, ~from: int, ~to_: int=?, ~type_: Model.NodeType.t, ~attrs: Model.Attrs.t=?, unit) => t =
      "setBlockType";
    [@bs.send]
    external setNodeMarkup:
      (
        t,
        ~pos: int,
        ~type_: Model.NodeType.t=?,
        ~attrs: Model.Attrs.t=?,
        ~marks: array(Model.Mark.t)=?,
        unit
      ) =>
      t =
      "setNodeMarkup";
    [@bs.send]
    external split:
      (
        t,
        ~pos: int,
        ~depth: int=?,
        ~typesAfter: array({
                       .
                       "_type": Model.NodeType.t,
                       "attrs": Model.Attrs.t,
                     })
                       =?,
        unit
      ) =>
      t =
      "split";
    [@bs.send] external join: (t, ~pos: int, ~depth: int=?, unit) => t = "join";
    /**
    The following helper functions can be useful when creating transformations or determining whether they are even possible.
    */
    [@bs.module "prosemirror-transform"] [@bs.return nullable] [@bs.val]
    external replaceStep:
      (~doc: Model.Node.t, ~from: int, ~to_: int=?, ~slice: Model.Slice.t=?, unit) =>
      option(Step.t) =
      "replaceStep";
    [@bs.module "prosemirror-transform"] [@bs.return nullable] [@bs.val]
    external liftTarget: (~range: Model.NodeRange.t) => option(int) = "liftTarget";

    [@bs.module "prosemirror-transform"] [@bs.return nullable] [@bs.val]
    external findWrapping:
      (
        ~range: Model.NodeRange.t,
        ~nodeType: Model.NodeType.t,
        ~attrs: Model.Attrs.t=?,
        ~innerRange: Model.NodeRange.t=?,
        unit
      ) =>
      option(
        array({
          .
          "_type": Model.NodeType.t,
          "attrs": Model.Attrs.t,
        }),
      ) =
      "findWrapping";
    [@bs.module "prosemirror-transform"] [@bs.val]
    external canSplit:
      (
        ~doc: Model.Node.t,
        ~pos: int,
        ~depth: int=?,
        ~typesAfter: array({
                       .
                       "_type": Model.NodeType.t,
                       "attrs": Model.Attrs.t,
                     })
                       =?,
        unit
      ) =>
      bool =
      "canSplit";
    [@bs.module "prosemirror-transform"] [@bs.val]
    external canJoin: (~doc: Model.Node.t, ~pos: int) => bool = "canJoin";
    [@bs.module "prosemirror-transform"] [@bs.return nullable] [@bs.val]
    external joinPoint: (~doc: Model.Node.t, ~pos: int, ~dir: int=?, unit) => option(int) =
      "joinPoint";
    [@bs.module "prosemirror-transform"] [@bs.return nullable] [@bs.val]
    external insertPoint:
      (~doc: Model.Node.t, ~pos: int, ~nodeType: Model.NodeType.t) => option(int) =
      "insertPoint";

    [@bs.module "prosemirror-transform"] [@bs.return nullable] [@bs.val]
    external dropPoint: (~doc: Model.Node.t, ~pos: int, ~silce: Model.Slice.t) => option(int) =
      "dropPoint";
  };

  type t;

  include Make({
    type nonrec t = t;
  });
};
