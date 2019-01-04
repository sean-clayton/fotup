module Option = {
  let unwrapUnsafely =
    fun
    | Some(v) => v
    | None => raise(Invalid_argument("unwrapUnsafely called on None"));
};

module DataTransfer = {
  type dropEffect = string;

  type effectAllowed = string;

  type file = Webapi.File.t;

  type files = array(file);

  type item = {kind: string};

  type items = array(item);

  type types = array(string);

  [@bs.deriving jsConverter]
  type dataTransfer = {
    dropEffect,
    effectAllowed,
    files,
    items,
    types,
  };
};

module FormData = {
  [@bs.new] external create: unit => Js.t('a) = "FormData";
  [@bs.send] external set: (Js.t('a), string, 'a) => Js.t('a) = "set";
};

module Location = {
  [@bs.val] [@bs.scope ("window", "location")]
  external assign: string => unit = "assign";
};

let redirect = newPath => {
  Location.assign(newPath);
};

module MouseEvent = {
  type t = ReactEvent.Mouse.t;

  [@bs.get] external dataTransfer: t => Js.t({..}) = "";
};

module Target = {
  type t = Js.t({.});

  [@bs.get] external files: t => array(Webapi.File.t) = "";
};
