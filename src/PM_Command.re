type ext =
  (
    . PM_State.EditorState.t,
    Js.Undefined.t(PM_State.Transaction.t => bool),
    Js.Undefined.t(PM_View.t)
  ) =>
  bool;

type t =
  (
    ~state: PM_State.EditorState.t,
    ~dispatch: PM_State.Transaction.t => bool=?,
    ~view: PM_View.t=?,
    unit
  ) =>
  bool;
