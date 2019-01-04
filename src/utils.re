module Option = {
  let unwrapUnsafely =
    fun
    | Some(v) => v
    | None => raise(Invalid_argument("unwrapUnsafely called on None"));
};

module File = {
  [@bs.deriving jsConverter]
  type file = {
    lastModified: int,
    lastModifiedDate: Js.Date.t,
    name: string,
    size: int,
    _type: string,
  };
};
