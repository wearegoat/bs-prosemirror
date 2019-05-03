type t;

[@bs.module "prosemirror-inputrules"] [@bs.new]
external make:
  (
    ~match: Js.Re.t,
    ~handler: [@bs.unwrap] [
                | `String(string)
                | `Fn(
                    (
                      ~state: PM_State.EditorState.t,
                      ~match: array(string),
                      ~start: int,
                      ~end_: int
                    ) =>
                    Js.Nullable.t(PM_State.Transaction.t),
                  )
              ]
  ) =>
  t =
  "";

[@bs.module "prosemirror-inputrules"]
external inputRulesExt: {. "rules": array(t)} => PM_State.Plugin.t = "inputRules";

let inputRules: array(t) => PM_State.Plugin.t = xs => inputRulesExt({"rules": xs});

[@bs.module "prosemirror-inputrules"] external undoInputRule: PM_Command.t = "undoInputRule";

[@bs.module "prosemirror-inputrules"] external emDash: t = "emDash";

[@bs.module "prosemirror-inputrules"] external ellipsis: t = "ellipsis";

[@bs.module "prosemirror-inputrules"] external openDoubleQuote: t = "openDoubleQuote";

[@bs.module "prosemirror-inputrules"] external closeDoubleQuote: t = "closeDoubleQuote";

[@bs.module "prosemirror-inputrules"] external openSingleQuote: t = "openSingleQuote";

[@bs.module "prosemirror-inputrules"] external closeSingleQuote: t = "closeSingleQuote";

[@bs.module "prosemirror-inputrules"] external smartQuotes: t = "smartQuotes";

[@bs.module "prosemirror-inputrules"]
external wrappingInputRule:
  (
    ~regexp: Js.Re.t,
    ~nodeType: PM_Model.NodeType.t,
    ~getAttrs: [@bs.unwrap] [
                 | `Attrs(PM_Model.Attrs.t)
                 | `Fn(array(string) => PM_Model.Attrs.t)
               ]
                 =?,
    ~joinPredicate: (array(string), PM_Model.Node.t) => bool=?,
    unit
  ) =>
  t =
  "";

[@bs.module "prosemirror-inputrules"]
external textblockTypeInputRule:
  (
    ~regexp: Js.Re.t,
    ~nodeType: PM_Model.NodeType.t,
    ~getAttrs: [@bs.unwrap] [
                 | `Attrs(PM_Model.Attrs.t)
                 | `Fn(array(string) => PM_Model.Attrs.t)
               ]
                 =?,
    unit
  ) =>
  t =
  "";
