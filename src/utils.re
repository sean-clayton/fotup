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
  [@bs.send] external append: (Js.t('a), string, 'a) => Js.t('a) = "append";
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

  [@bs.get] external dataTransfer: t => Js.t({..}) = "dataTransfer";
};

module Target = {
  type t = Js.t({.});

  [@bs.get] external files: t => array(Webapi.File.t) = "files";
};

module ProgressEvent = {
  type lengthComputable = bool;
  type loaded = int;
  type total = int;

  [@bs.deriving jsConverter]
  type progressEvent = {
    lengthComputable,
    loaded,
    total,
  };
};

module Environment = {
  [@bs.val]
  external commitRef: Js.Undefined.t(string) = "process.env.COMMIT_REF";

  [@bs.val]
  external imageHostPath: Js.Undefined.t(string) =
    "process.env.IMAGE_HOST_PATH";
};
