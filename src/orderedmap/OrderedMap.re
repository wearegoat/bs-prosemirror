type t('a);

[@bs.module "orderedmap"] external make: Js.Dict.t('a) => t('a) = "from";

[@bs.return nullable] [@bs.send] external get: (t('a), string) => option('a) = "";
[@bs.send]
external update: (t('a), ~key: string, ~value: 'a, ~newKey: string=?, unit) => t('a) = "";
[@bs.send] external remove: (t('a), ~key: string) => t('a) = "";
[@bs.send] external addToStart: (t('a), ~key: string, ~value: 'a) => t('a) = "";
[@bs.send] external addToEnd: (t('a), ~key: string, ~value: 'a) => t('a) = "";
[@bs.send] external addBefore: (t('a), ~place: string, ~key: string, ~value: 'a) => t('a) = "";
[@bs.send] external forEach: (t('a), (~key: string, ~value: 'a) => unit) => unit = "";
[@bs.send] external prepend: (t('a), ~map: t('a)) => t('a) = "";
[@bs.send] external append: (t('a), ~map: t('a)) => t('a) = "";
[@bs.send] external subtract: (t('a), ~map: t('a)) => t('a) = "";
[@bs.get] external size: t('a) => int = "";
