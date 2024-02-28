import API_Generic from "./API_Generic";
export default function API_Execution(setLogout, addAlert) {
  function getExecutions(
    id,
    token,
    body,
    setObject,
    processing_function,
    post_function
  ) {
    API_Generic(setLogout, addAlert).genericCall(
      "/campaigns/executions/" + id,
      API_Generic(setLogout, addAlert).requestOptions("GET", token, body),
      false,
      setObject,
      processing_function,
      post_function
    );
  }

  function deleteExecution(
    id,
    token,
    idexecution,
    setObject,
    processing_function,
    post_function
  ) {
    let body = {"execution_id" : idexecution};

    API_Generic(setLogout, addAlert).genericCall(
      "/campaigns/executions/" + id ,
      API_Generic(setLogout, addAlert).requestOptions("DELETE", token, body),
      true,
      setObject,
      processing_function,
      post_function
    );
  }

  function createExecution(
    token,
    body,
    setObject,
    processing_function,
    post_function
  ) {
    API_Generic(setLogout, addAlert).genericCall(
      "/campaigns/executions",
      API_Generic(setLogout, addAlert).requestOptions("POST", token, body),
      true,
      setObject,
      processing_function,
      post_function
    );
  }

  return {
    getExecutions,
    createExecution,
    deleteExecution,
  };
}
