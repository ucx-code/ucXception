import React, { useState, useContext, useEffect } from "react";
import { useNavigate, useLocation } from "react-router-dom";
import AuthContext from "../../context/AuthProvider";
import AlertsContext from "../../context/AlertsProvider";
import API_Host from "../../utils/api/API_Host";
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
  CFormSwitch,
  CFormLabel,
} from "@coreui/react";
import CIcon from "@coreui/icons-react";
import { cilWarning, cilTrash, cilCheck, cilX } from "@coreui/icons";
import "@coreui/coreui/dist/css/coreui.min.css";

const CampaignExecution = () => {
  //Authenticaton
  const { isAuthenticated, handleLogout } = useContext(AuthContext);
  const { setAlert, addAlert } = useContext(AlertsContext);
  const token = localStorage.getItem("token");
  const [logout, setLogout] = useState(false);

  //Nagivation
  const navigate = useNavigate();
  const location = useLocation();
  const from_login = location.state?.from?.pathname || "/login";
  const from_menu = location.state?.from?.pathname || "/menu";
  const from_component = location.state?.from?.pathname || "/preprobes";

  //Valdiate form
  const [validated, setValidated] = useState(false);
  const [visible, setVisible] = useState(false);

  //Campaign ID
  const [campaignId, setCampaignId] = useState();
  const [isRemote, setIsRemote] = useState(false);

  //List of executions
  const [hosts, setHosts] = useState();

  //Has targets
  const [hasCampaignTarget, setHasCampaignTarget] = useState(false);
  const [hasFaultInjectorTarget, setHasFaultInjectorTarget] = useState(false);

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
      setAlert({});
      setCampaignId(location.state["campaign_id"]);
      handleGetHosts();
    }
  }, []);

  //Function will call api to get hosts data
  const handleGetHosts = () => {
    API_Host(setLogout, addAlert).getHosts(
      location.state["campaign_id"],
      token,
      null,
      setHosts,
      prefunctionHosts,
      null
    );
  };

  const prefunctionHosts = (values) => {
    if (values !== undefined && values.length > 0) {
      if (values[0]["campaign_target"] !== null) {
        setHasCampaignTarget(true);
      } else {
        setHasCampaignTarget(false);
      }

      if (values[0]["campaign_fault_injector_target"] !== null) {
        setHasFaultInjectorTarget(true);
      } else {
        setHasFaultInjectorTarget(false);
      }
    }
    return values;
  };

  //Function will call api to delete host
  const handleDeleteHost = (e, idhost) => {
    API_Host(setLogout, addAlert).deleteHost(
      campaignId,
      token,
      idhost,
      null,
      null,
      handleGetHosts
    );
  };

  //Function will call api to create host
  const handleCreateHost = (body) => {
    API_Host(setLogout, addAlert).createHost(
      token,
      body,
      null,
      null,
      handleGetHosts
    );
  };

  //Set card visible to allow the creation of an execuun
  const handleExecutionForm = () => {
    setVisible(true);
  };

  const handleChangeHostType = (e, value) => {
    setIsRemote(value);
  };

  const handleChangePage = () => {
    setAlert({});
    navigate(from_component, {
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

      const host = {
        campaign_id: campaignId,
        type: data.get("gridRadios"),
        domain: data.get("domain"),
        username: data.get("username"),
        campaign_target: data.get("campaign_target") === "true",
        fault_injector_target: data.get("fault_injector_target") === "true",
      };

      handleCreateHost(host);

      setVisible(false);
      setIsRemote(false);
      setValidated(false);
    }
  };

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
            <CCol sm={11} md={10} lg={8}>
              <CCard>
                <CCardHeader className="text-center">
                  <h4>Hosts</h4>
                </CCardHeader>
                <CCardBody>
                  <CTable responsive>
                    <CTableHead>
                      <CTableRow>
                        <CTableHeaderCell scope="col">Host ID</CTableHeaderCell>
                        <CTableHeaderCell scope="col">
                          Campaign target
                        </CTableHeaderCell>
                        <CTableHeaderCell scope="col">
                          Fault injector target
                        </CTableHeaderCell>
                        <CTableHeaderCell scope="col">
                          Host type
                        </CTableHeaderCell>
                        <CTableHeaderCell scope="col">Domain</CTableHeaderCell>
                        <CTableHeaderCell scope="col">
                          Username
                        </CTableHeaderCell>
                        <CTableHeaderCell scope="col"></CTableHeaderCell>
                      </CTableRow>
                    </CTableHead>
                    <CTableBody>
                      {hosts !== undefined && hosts !== null
                        ? hosts.map((object, key) => (
                            <CTableRow key={key}>
                              <CTableHeaderCell scope="row">
                                {object["idhost"]}
                              </CTableHeaderCell>
                              <CTableHeaderCell scope="row">
                                {object["campaign_target"] ===
                                object["idhost"] ? (
                                  <CIcon
                                    icon={cilCheck}
                                    size="lg"
                                    className="my-1 mx-2"
                                  />
                                ) : (
                                  <CIcon
                                    icon={cilX}
                                    size="lg"
                                    className="my-1 mx-2"
                                  />
                                )}
                              </CTableHeaderCell>
                              <CTableHeaderCell scope="row">
                                {object["campaign_fault_injector_target"] ===
                                object["idhost"] ? (
                                  <CIcon
                                    icon={cilCheck}
                                    size="lg"
                                    className="my-1 mx-2"
                                  />
                                ) : (
                                  <CIcon
                                    icon={cilX}
                                    size="lg"
                                    className="my-1 mx-2"
                                  />
                                )}
                              </CTableHeaderCell>
                              <CTableDataCell>
                                {object["type"] === "localhost" ? (
                                  <p>Localhost</p>
                                ) : (
                                  <p>Remote host</p>
                                )}
                              </CTableDataCell>
                              <CTableDataCell>
                                {object["domain"]}
                              </CTableDataCell>
                              <CTableDataCell>
                                {object["username"]}
                              </CTableDataCell>
                              <CTableDataCell>
                                <CButton
                                  color="danger"
                                  variant="ghost"
                                  onClick={(e) =>
                                    handleDeleteHost(e, object["idhost"])
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
                    <CCol xs={6}>
                      <CButton color="primary" onClick={handleExecutionForm}>
                        Create host
                      </CButton>
                    </CCol>
                    {hosts !== undefined &&
                    hosts.length !== 0 &&
                    hasCampaignTarget === true &&
                    hasFaultInjectorTarget === true ? (
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
              <CCol sm={11} md={10} lg={4}>
                <CCard>
                  <CCardHeader className="text-center">
                    <h5>Create host</h5>
                  </CCardHeader>
                  <CCardBody>
                    <CRow xs={{ gutterY: 10 }}>
                      <fieldset className="row mb-3">
                        <h6>Host Type</h6>
                        <CCol sm={10}>
                          <CFormCheck
                            type="radio"
                            name="gridRadios"
                            id="gridRadios1"
                            value="localhost"
                            label="Localhost"
                            defaultChecked
                            onClick={(e) => handleChangeHostType(e, false)}
                          />
                          <CFormCheck
                            type="radio"
                            name="gridRadios"
                            id="gridRadios2"
                            value="remote"
                            label="Remote host"
                            onClick={(e) => handleChangeHostType(e, true)}
                          />
                        </CCol>
                      </fieldset>

                      <CFormLabel htmlFor="exampleFormControlTextarea1">
                        <h6>Define targets</h6>
                        <CFormSwitch
                          label="Campaign target"
                          name="campaign_target"
                          id="campaign_target"
                          value={true}
                        />
                        <CFormSwitch
                          label="Fault injector target"
                          name="fault_injector_target"
                          id="fault_injector_target"
                          value={true}
                        />
                      </CFormLabel>

                      {isRemote ? (
                        <div>
                          {CreateFields().createInputText(
                            2,
                            "8",
                            "Domain",
                            "domain",
                            "domain",
                            true,
                            1,
                            20,
                            "",
                            "",
                            null,
                            true,
                            "Execution must have a domain!"
                          )}
                          {CreateFields().createInputText(
                            3,
                            "8",
                            "Username",
                            "username",
                            "username",
                            true,
                            1,
                            20,
                            "",
                            "",
                            null,
                            true,
                            "Must have a username!"
                          )}
                        </div>
                      ) : null}
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
