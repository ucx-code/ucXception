import API_Generic from "./API_Generic";
export default function API_Host(setLogout, addAlert) {
  function getHosts(
    id,
    token,
    body,
    setObject,
    processing_function,
    post_function
  ) {
    API_Generic(setLogout, addAlert).genericCall(
      "/campaigns/hosts/" + id,
      API_Generic(setLogout, addAlert).requestOptions("GET", token, body),
      false,
      setObject,
      processing_function,
      post_function
    );
  }

  function deleteHost(
    id,
    token,
    idhost,
    setObject,
    processing_function,
    post_function
  ) {
    let body = {"host_id" : idhost};

    API_Generic(setLogout, addAlert).genericCall(
      "/campaigns/hosts/" + id,
      API_Generic(setLogout, addAlert).requestOptions("DELETE", token, body),
      true,
      setObject,
      processing_function,
      post_function
    );
  }

  function createHost(
    token,
    body,
    setObject,
    processing_function,
    post_function
  ) {
    API_Generic(setLogout, addAlert).genericCall(
      "/campaigns/hosts",
      API_Generic(setLogout, addAlert).requestOptions("POST", token, body),
      true,
      setObject,
      processing_function,
      post_function
    );
  }

  return {
    getHosts,
    deleteHost,
    createHost,
  };
}
