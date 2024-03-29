import API_Generic from "./API_Generic";
export default function API_Campaign(setLogout, addAlert) {
  function getCampaignsData(
    token,
    body,
    setObject,
    processing_function,
    post_function
  ) {
    API_Generic(setLogout, addAlert).genericCall(
      "/campaigns/information",
      API_Generic(setLogout, addAlert).requestOptions("GET", token, body),
      false,
      setObject,
      processing_function,
      post_function
    );
  }

  function sendUserCampaign(
    token,
    body,
    setObject,
    processing_function,
    post_function,
    getHeaders
  ) {
    const requestOptions = {
      method: "POST",
      headers: { Authentication: token },
      body: body,
    };

    API_Generic(setLogout, addAlert).genericCall(
      "/campaigns",
      requestOptions,
      true,
      setObject,
      processing_function,
      post_function,
      getHeaders
    );
  }
  
  function deleteCampaign(
    id,
    token,
    setObject,
    processing_function,
    post_function
  ) {
    API_Generic(setLogout, addAlert).genericCall(
      "/campaigns/" + id,
      API_Generic(setLogout, addAlert).requestOptions("DELETE", token, null),
      true,
      setObject,
      processing_function,
      post_function
    );
  }

  return {
    getCampaignsData,
    sendUserCampaign,
    deleteCampaign,
  };
}
