import API_Generic from "./API_Generic";
export default function API_Display(setLogout, addAlert) {
  function getCampaignStatistics(id, token, processing_function) {
    API_Generic(setLogout, addAlert).genericCall(
      "/campaign/" + id + "/statistics",
      API_Generic(setLogout, addAlert).requestOptions("GET", token, null),
      false,
      null,
      processing_function,
      null
    );
  }

  function getCampaignCharts(id, token, body, setObject) {
    API_Generic(setLogout, addAlert).genericCall(
      "/campaign/" + id + "/chart",
      API_Generic(setLogout, addAlert).requestOptions("POST", token, body),
      false,
      setObject,
      null,
      null
    );
  }

  function getDownloadFile(id, token) {
    API_Generic(setLogout, addAlert).genericCallBlob(
      "/campaign/" + id + "/download",
      API_Generic(setLogout, addAlert).requestOptions("GET", token, null)
    );
  }

  function getColumnsCsvfile(id, token, preFunction) {
    API_Generic(setLogout, addAlert).genericCall(
      "/campaign/" + id + "/columns",
      API_Generic(setLogout, addAlert).requestOptions("GET", token, null),
      false,
      null,
      preFunction,
      null
    );
  }

  return {
    getCampaignStatistics,
    getCampaignCharts,
    getDownloadFile,
    getColumnsCsvfile,
  };
}
