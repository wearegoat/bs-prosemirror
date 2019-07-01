module Command = PM_Command;
[@bs.module "prosemirror-history"] external undo: Command.t = "";

[@bs.module "prosemirror-history"] external redo: Command.t = "";

[@bs.module "prosemirror-history"] external undoDepth: PM_State.EditorState.t => int = "";

[@bs.module "prosemirror-history"] external redoDepth: PM_State.EditorState.t => int = "";

[@bs.module "prosemirror-history"]
external closeHistory: PM_State.Transaction.t => PM_State.Transaction.t = "";

[@bs.deriving abstract]
type config = {
  [@bs.optional]
  depth: int,
  [@bs.optional]
  newGroupDelay: int,
};

[@bs.module "prosemirror-history"]
external history: (~config: config=?, unit) => PM_State.Plugin.t('a) = "";
