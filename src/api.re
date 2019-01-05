open Utils;

let uploadEndpoint = "https://api.put.re/upload/";

let uploadFile = (~onUploadProgress=?, file: Webapi.File.t) => {
  let formData = FormData.create();
  let _ = formData->FormData.set("file", file);
  switch (onUploadProgress) {
  | Some(handler) =>
    Axios.makeConfig(~onUploadProgress=handler, ())
    |> Axios.postDatac(uploadEndpoint, formData)
  | None => Axios.postData(uploadEndpoint, formData)
  };
};
