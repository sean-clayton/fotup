open Utils;

let uploadEndpoint = "https://api.put.re/upload/";

let uploadFile = (file: Webapi.File.t) => {
  let formData = FormData.create();
  let _ = formData->FormData.set("file", file);
  Axios.postData(uploadEndpoint, formData);
};
