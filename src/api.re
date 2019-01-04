let uploadEndpoint = "https://api.put.re/upload/";

let unsafeCreateFormData = [%raw
  {|
  function(file) {
    let data = new FormData();
    data.append('file', file);

    return data;
  }
|}
];

let uploadFile = (file: Webapi.File.t) => {
  let formData = unsafeCreateFormData(file);
  Axios.postData(uploadEndpoint, formData);
};
