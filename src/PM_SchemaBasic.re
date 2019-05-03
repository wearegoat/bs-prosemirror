[@bs.module "prosemirror-schema-basic"] external nodes: Js.Dict.t(PM_Model.NodeSpec.t) = "";

[@bs.module "prosemirror-schema-basic"] external marks: Js.Dict.t(PM_Model.MarkSpec.t) = "";

[@bs.module "prosemirror-schema-basic"] external schema: PM_Model.Schema.t = "";
