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
  
  // Function to delete a Campaign
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

  // Function to get information about a single campaign
  function get_campaign(
    id,
    token,
    setObject,
    processing_function,
    post_function
  ) {
    API_Generic(setLogout, addAlert).genericCall(
      "/campaigns/" + id,
      API_Generic(setLogout, addAlert).requestOptions("GET", token, null),
      true,
      setObject,
      processing_function,
      post_function
    );
  }

  // Function that sends the form data (app_input, campaign_type and file) to run a suggestion campaign
  function sendCampaignFormData(
    token, 
    formData, 
    setObject, 
    processing_function, 
    post_function
  ) {
    const requestOptions = {
      method: "POST",
      headers: { Authentication: token },
      body: formData,
    };
    API_Generic(setLogout, addAlert).genericCall(
      "/campaign/setup/suggestion_run",
      requestOptions,
      true,
      setObject,
      processing_function,
      post_function
    );
  }

  function archiveCampaign(
    id,
    token,
    setObject,
    processing_function,
    post_function
  ) {
    API_Generic(setLogout, addAlert).genericCall(
      "/campaigns/archive/" + id,
      API_Generic(setLogout, addAlert).requestOptions("PUT", token, null),
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
    get_campaign,
    sendCampaignFormData,
    archiveCampaign,
  };
}
