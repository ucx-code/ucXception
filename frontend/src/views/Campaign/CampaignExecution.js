import React, { useState, useContext, useEffect } from "react";
import { useNavigate, useLocation } from "react-router-dom";
import AuthContext from "../../context/AuthProvider";
import AlertsContext from "../../context/AlertsProvider";
import API_Execution from "../../utils/api/API_Execution";
import CreateFields from "../../utils/CreateFields";
import AppBreadcrumb from "../../components/AppBreadcrumb";
import AppAlert from "../../components/AppAlerts";
import {
  CButton,
  CCard,
  CCardBody,
  CCol,
  CContainer,
  CCardHeader,
  CForm,
  CRow,
  CTable,
  CTableHead,
  CTableRow,
  CTableHeaderCell,
  CTableBody,
  CTableDataCell,
  CFormCheck,
} from "@coreui/react";
import CIcon from "@coreui/icons-react";
import { cilWarning, cilTrash } from "@coreui/icons";
import "@coreui/coreui/dist/css/coreui.min.css";

const CampaignExecution = () => {
  //Authenticaton
  const { isAuthenticated, handleLogout } = useContext(AuthContext);
  const { addAlert, setAlert } = useContext(AlertsContext);
  const token = localStorage.getItem("token");
  const [logout, setLogout] = useState(false);

  //Nagivation
  const navigate = useNavigate();
  const location = useLocation();
  const from_login = location.state?.from?.pathname || "/login";
  const from_menu = location.state?.from?.pathname || "/menu";
  const from_hosts = location.state?.from?.pathname || "/campaign/hosts";

  //Valdiate form
  const [validated, setValidated] = useState(false);
  const [visible, setVisible] = useState(false);

  //Campaign ID
  const [campaignId, setCampaignId] = useState();

  //List of executions
  const [executions, setExecutions] = useState();

  // Head of Campaign Information copied from local Storage
  const head = JSON.parse(localStorage.getItem("Copied"));

  useEffect(() => {
    if (logout === true) {
      handleLogout();
      navigate(from_login, { replace: true });
    }
  }, [logout]);

  useEffect(() => {
    if (!isAuthenticated()) {
      addAlert("Need to login first!", "warning", cilWarning);
      navigate(from_login, { replace: true });
    }

    if (location.state === null || location.state === undefined) {
      addAlert("Need to start creating a campaign!", "warning", cilWarning);
      navigate(from_menu, { replace: true });
    } else {
      setCampaignId(location.state["campaign_id"]);

      handleGetExecutions();
    }
  }, []);

  //Function will call api to get executions data
  const handleGetExecutions = () => {
    API_Execution(setLogout, addAlert).getExecutions(
      location.state["campaign_id"],
      token,
      null,
      setExecutions,
      null,
      null
    );
  };

  //Function will call api to delete execution
  const handleDeleteExecution = (e, idexecution) => {
    API_Execution(setLogout, addAlert).deleteExecution(
      campaignId,
      token,
      idexecution,
      null,
      null,
      handleGetExecutions
    );
  };

  //Function will call api to create execution
  const handleCreateExecution = (body) => {
    API_Execution(setLogout, addAlert).createExecution(
      token,
      body,
      null,
      null,
      handleGetExecutions
    );
  };

  //Set card visible to allow the creation of an execuun
  const handleExecutionForm = () => {
    setVisible(true);
  };

  const handleChangePage = () => {
    setAlert({});

    navigate(from_hosts, {
      state: { campaign_id: location.state["campaign_id"] },
      replace: true,
    });
  };

  const handleSubmit = (event) => {
    const form = event.currentTarget;
    event.preventDefault();

    if (form.checkValidity() === false) {
      event.stopPropagation();
      setValidated(true);
    } else {
      const data = new FormData(event.target);

      const execution = {
        campaign_id: campaignId,
        type: data.get("gridRadios") === "true",
        name: data.get("execution_name"),
        n_target_runs: parseInt(data.get("number_runs"), 10),
      };

      handleCreateExecution(execution);
      setVisible(false);
      setValidated(false);
    }
  };

  // This will retrieve data from local storage and create the hosts from the selected campaign
  useEffect(() => {
    if (head && campaignId!== undefined) {
      for (let i = 0; i < head['Executions'].length; i++){
        {(head['Executions'][i]['hasfault'] === 1)? (head['Executions'][i]['hasfault'] = true) : (head['Executions'][i]['hasfault'] = false)};
        const execution = {
          campaign_id: campaignId,
          type: head['Executions'][i]['hasfault'],
          name: head['Executions'][i]['name'],
          n_target_runs: head['Executions'][i]['ntargetruns'],
        };
        handleCreateExecution(execution);
      }
    }
  }, [campaignId]);

  return (
    <>
      <CContainer>
        <CForm
          className="row g-2 needs-validation"
          noValidate
          validated={validated}
          onSubmit={handleSubmit}
        >
          <CRow className="justify-content-evenly">
            <AppAlert />
            <AppBreadcrumb />
            <CCol>
              <CCard>
                <CCardHeader className="text-center">
                  <h4>Executions</h4>
                </CCardHeader>
                <CCardBody>
                  <CTable>
                    <CTableHead>
                      <CTableRow>
                        <CTableHeaderCell scope="col">
                          Execution ID
                        </CTableHeaderCell>
                        <CTableHeaderCell scope="col">
                          Execution Name
                        </CTableHeaderCell>
                        <CTableHeaderCell scope="col">
                          Type of execution
                        </CTableHeaderCell>
                        <CTableHeaderCell scope="col">
                          Number Runs
                        </CTableHeaderCell>
                        <CTableHeaderCell scope="col"></CTableHeaderCell>
                      </CTableRow>
                    </CTableHead>
                    <CTableBody>
                      {executions !== undefined && executions !== null
                        ? executions.map((object, key) => (
                            <CTableRow key={key}>
                              <CTableHeaderCell scope="row">
                                {object["idexecution"]}
                              </CTableHeaderCell>
                              <CTableDataCell>{object["name"]}</CTableDataCell>
                              <CTableDataCell>
                                {object["hasfault"] ? (
                                  <p>Fault Injection</p>
                                ) : (
                                  <p>Golden Run</p>
                                )}
                              </CTableDataCell>
                              <CTableDataCell>
                                {object["ntargetruns"]}
                              </CTableDataCell>
                              <CTableDataCell>
                                <CButton
                                  color="danger"
                                  variant="ghost"
                                  onClick={(e) =>
                                    handleDeleteExecution(
                                      e,
                                      object["idexecution"]
                                    )
                                  }
                                >
                                  <CIcon icon={cilTrash} size="lg" />
                                </CButton>
                              </CTableDataCell>
                            </CTableRow>
                          ))
                        : null}
                    </CTableBody>
                  </CTable>

                  <CRow>
                    <CCol sm={6}>
                      <CButton color="primary" onClick={handleExecutionForm}>
                        Create execution
                      </CButton>
                    </CCol>
                    {executions !== undefined && executions.length !== 0 ? (
                      <CCol sm={6} className="text-end">
                        <CButton color="primary" onClick={handleChangePage}>
                          Next
                        </CButton>
                      </CCol>
                    ) : null}
                  </CRow>
                </CCardBody>
              </CCard>
            </CCol>

            {visible ? (
              <CCol sm={5}>
                <CCard>
                  <CCardHeader className="text-center">
                    <h5>Create execution</h5>
                  </CCardHeader>
                  <CCardBody>
                    <CRow xs={{ gutterY: 10 }}>
                      <fieldset className="row mb-3">
                        <h6>Execution Type</h6>
                        <CCol sm={10}>
                          <CFormCheck
                            type="radio"
                            name="gridRadios"
                            id="gridRadios1"
                            value={true}
                            label="Fault injection"
                            defaultChecked
                          />
                          <CFormCheck
                            type="radio"
                            name="gridRadios"
                            id="gridRadios2"
                            value={false}
                            label="Golden run"
                          />
                        </CCol>
                      </fieldset>
                      {CreateFields().createInputText(
                        2,
                        "8",
                        "Execution Name",
                        "execution_name",
                        "execution_name",
                        true,
                        1,
                        20,
                        "",
                        "",
                        null,
                        true,
                        "Execution must have a name!"
                      )}
                      {CreateFields().createInputNumber(
                        3,
                        "8",
                        "Number of Runs",
                        "number_runs",
                        "number_runs",
                        true,
                        1,
                        1,
                        10000,
                        1,
                        1,
                        null,
                        true,
                        "Must be bigger than zero!"
                      )}
                      <CCol xs={12} className="text-center">
                        <CButton color="primary" type="submit">
                          Create
                        </CButton>
                      </CCol>
                    </CRow>
                  </CCardBody>
                </CCard>
              </CCol>
            ) : null}
          </CRow>
        </CForm>
      </CContainer>
    </>
  );
};

export default CampaignExecution;
