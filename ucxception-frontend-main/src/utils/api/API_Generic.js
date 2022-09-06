import { cilBurn, cilCheckCircle } from "@coreui/icons";

export default function API_Generic(setLogout, addAlert) {
  function requestOptions(method, token, body) {
    const requestOptions = {
      headers: {
        "Content-Type": "application/json",
        Accept: "application/json",
      },
    };

    if (method) {
      requestOptions["method"] = method;
    }

    if (token) {
      requestOptions["headers"]["Authentication"] = token;
    }

    if (body) {
      requestOptions["body"] = JSON.stringify(body);
    }

    return requestOptions;
  }

  //get components names
  function genericCall(
    url,
    requestOptions,
    hasSucess,
    setObject,
    pre_processing,
    post_function,
    getHeaders
  ) {
    fetch(process.env.REACT_APP_API_URL + url, requestOptions)
      .then((response) => {
        if (
          response.status === 200 ||
          response.status === 201 ||
          response.status === 202
        ) {
          response.json().then((values) => {
            if (hasSucess === true) {
              addAlert(values["message"], "success", cilCheckCircle);
            }

            if (pre_processing !== null) {
              values = pre_processing(values);
            }

            if (setObject !== null) {
              setObject(values);
            }

            if (post_function !== null) {
              post_function();
            }
          });
          if (getHeaders) {
            getHeaders(response.headers);
          }
        } else if (response.status === 401) {
          response.json().then((values) => {
            addAlert(values["message"], "danger", cilBurn);
            if (setLogout !== null) {
              setLogout(true);
            }
          });
        } else {
          response.json().then((values) => {
            addAlert(values["message"], "danger", cilBurn);
          });
        }
      })
      .catch((error) => {
        console.log(error);
        addAlert("Internal error server", "danger", cilBurn);
      });
  }

  function genericCallBlob(url, requestOptions) {
    fetch(process.env.REACT_APP_API_URL + url, requestOptions)
      .then((response) => {
        if (response.status === 200 || response.status === 202) {
          response.blob().then((blob) => {
            // Create blob link to download
            const url = window.URL.createObjectURL(new Blob([blob]));
            const link = document.createElement("a");
            link.href = url;
            link.setAttribute("download", `data.csv`);

            // Append to html link element page
            document.body.appendChild(link);

            // Start download
            link.click();

            // Clean up and remove the link
            link.parentNode.removeChild(link);
          });
        } else if (response.status === 401) {
          response.json().then((values) => {
            addAlert(values["message"], "danger", cilBurn);
            if (setLogout !== null) {
              setLogout(true);
            }
          });
        } else {
          response.json().then((values) => {
            addAlert(values["message"], "danger", cilBurn);
          });
        }
      })
      .catch((error) => {
        addAlert("Internal error server", "danger", cilBurn);
      });
  }

  return {
    requestOptions,
    genericCall,
    genericCallBlob,
  };
}
