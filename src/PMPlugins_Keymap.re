module Command = PM_Command;

[@bs.module "prosemirror-keymap"]
external keymap: Js.Dict.t(Command.t) => PM_State.Plugin.t('a) = "keymap";

[@bs.module "prosemirror-keymap"]
external keydownHandler: Js.Dict.t(Command.t) => Command.t = "keydownHandler";
