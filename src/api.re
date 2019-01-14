open Utils;

let uploadEndpoint = "https://api.put.re/upload/";

let uploadFile = (~onUploadProgress=?, file: Webapi.File.t) => {
  let formData = FormData.create();
  let _ = formData->FormData.append("file", file);
  switch (onUploadProgress) {
  | Some(handler) =>
    Axios.makeConfig(~onUploadProgress=handler, ())
    |> Axios.postDatac(uploadEndpoint, formData)
  | None => Axios.postData(uploadEndpoint, formData)
  };
};

[@bs.deriving jsConverter]
type upload = {
  originalName: string,
  name: string,
  extension: string,
  deleteToken: string,
  size: int,
  thumbnailLink: option(string),
  link: string,
  deleteLink: string,
};
