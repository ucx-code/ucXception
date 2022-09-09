import API_Generic from "./API_Generic";
export default function API_Components(setLogout, addAlert) {
  function getComponentsNames(
    location,
    token,
    body,
    setObject,
    processing_function,
    post_function
  ) {
    API_Generic(setLogout, addAlert).genericCall(
      "/components/" + location,
      API_Generic().requestOptions("GET", token, body),
      false,
      setObject,
      processing_function,
      post_function
    );
  }

  function getUserComponents(
    id,
    token,
    body,
    setObject,
    processing_function,
    post_function
  ) {
    API_Generic(setLogout, addAlert).genericCall(
      "/components/user/" + id,
      API_Generic().requestOptions("GET", token, body),
      false,
      setObject,
      processing_function,
      post_function
    );
  }

  function getUserHosts(
    id,
    token,
    body,
    setObject,
    processing_function,
    post_function
  ) {
    API_Generic(setLogout, addAlert).genericCall(
      "/campaigns/hosts/" + id,
      API_Generic().requestOptions("GET", token, body),
      false,
      setObject,
      processing_function,
      post_function
    );
  }

  function getComponentsData(
    location,
    target,
    token,
    body,
    setObject,
    processing_function,
    post_function
  ) {
    API_Generic(setLogout, addAlert).genericCall(
      "/components/" + location + "/" + target,
      API_Generic().requestOptions("GET", token, body),
      false,
      setObject,
      processing_function,
      post_function
    );
  }

  function executeCampaign(
    token,
    body,
    setObject,
    processing_function,
    post_function
  ) {
    API_Generic(setLogout, addAlert).genericCall(
      "/execute",
      API_Generic().requestOptions("POST", token, body),
      false,
      setObject,
      processing_function,
      post_function
    );
  }

  return {
    getComponentsNames,
    getUserComponents,
    getUserHosts,
    getComponentsData,
    executeCampaign,
  };
}
