import React, { useState, useContext, useEffect } from "react";
import { useNavigate, useLocation } from "react-router-dom";
import AuthContext from "../../context/AuthProvider";
import AlertsContext from "../../context/AlertsProvider";
import API_Campaign from "../../utils/api/API_Campaign";
import CreateFields from "../../utils/CreateFields";
import CampaignParameters from "./CampaignParameters";
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
  CFormInput,
  CRow,
  CFormLabel,
  CPopover,
} from "@coreui/react";
import CIcon from "@coreui/icons-react";
import { cilWarning, cilInfo } from "@coreui/icons";
import "@coreui/coreui/dist/css/coreui.min.css";

const CampaignSetup = () => {
  //Authenticaton
  const { isAuthenticated, handleLogout } = useContext(AuthContext);
  const { addAlert, setAlert } = useContext(AlertsContext);
  const token = localStorage.getItem("token");

  //Nagivation
  const navigate = useNavigate();
  const location = useLocation();
  const from_login = location.state?.from?.pathname || "/login";
  const from_execution =
    location.state?.from?.pathname || "/campaign/executions";

  //Valdiate form
  const [validated, setValidated] = useState(false);
  const [logout, setLogout] = useState(false);

  //Campaign information API and input
  const [campaignsApiData, setApiCampaignsData] = useState();
  const [campaignType, setCampaignType] = useState("");

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
    //Set alerts to empty
    setAlert({});
    //Get campaign data
    API_Campaign(setLogout, addAlert).getCampaignsData(
      token,
      null,
      setApiCampaignsData,
      setFirstCampaignType,
      null
    );
  }, []);

  // Handles change type of campaign
  const handleCampaignChange = (name, value) => {
    setCampaignType(value);
  };

  // Function used to set first campaign type
  function setFirstCampaignType(values) {
    if (values) {
      setCampaignType((null != head) ? (head["Campaign_Type"]):(values[0]["campaign_name"]));
    }
    return values;
  }

  function changePage(values) {
    setAlert({});
    navigate(from_execution, {
      state: { campaign_id: values["campaign-id"] },
      replace: true,
    });
  }

  const handleSubmit = (event) => {
    const form = event.currentTarget;
    event.preventDefault();
    if (form.checkValidity() === false) {
      event.stopPropagation();
    } else {
      const data = new FormData(form);

      //Iterate campaign data
      for (let i = 0; i < campaignsApiData.length; i++) {
        //Search for selected campaign type
        if (
          campaignsApiData[i]["campaign_name"] === data.get("campaign_type")
        ) {
          //Get parameters from campaign type choosed
          Object.keys(campaignsApiData[i]["parameters"]).forEach(function (
            key
          ) {
            let aux;
            if (campaignsApiData[i]["parameters"][key]["type"] === "multiple") {
              aux = data.getAll(key);
              data.set(key, aux);
            } else {
              aux = data.get(key);
              //Change on to true (checkbox)
              if (aux) {
                if (aux === "on") {
                  data.set(key, true);
                }

                //If not (normaly checkbox not selected) we define that key with default value or false
              } else {
                if (campaignsApiData[i]["parameters"][key]["default"]) {
                  data.append(
                    key,
                    campaignsApiData[i]["parameters"][key]["default"]
                  );
                } else {
                  data.append(key, false);
                }
              }
            }
          });
        }
      }

      //Send user campaign data
      API_Campaign(setLogout, addAlert).sendUserCampaign(
        token,
        data,
        null,
        changePage,
        null
      );
    }

    setValidated(true);
  };

  // Function that will replace app_input / f_min / f_max / watchdog_dur depending if there is Copied information on localStorage
  function function_campaignsApiData(name,array){
    if (array != null){
      for(let i = 0; i < array.length; i++){
        if (array[i]["campaign_name"] === name && head != null){
          array[i]["parameters"]["app_input"]["default"] = head["app_input"]
          array[i]["parameters"]["fi_max"]["default"] = head["fi_max"]
          array[i]["parameters"]["fi_min"]["default"] = head["fi_min"]
          array[i]["parameters"]["watchdog_dur"]["default"] = head["watchdog_dur"]
        }
      }
    }
    return array
  } 

  // Exibes a message if there is anything in copied in LocalStorage
  useEffect(() => {
    if ( head != null)
      addAlert("To delete the copied values, click on the 'Home' icon", "warning", cilWarning);
    return () => {
      setAlert({});
    }
  }, []);

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
            <CCol sm={5}>
              <CCard>
                <CCardHeader className="text-center">
                  <h4>Setup</h4>
                </CCardHeader>
                <CCardBody>
                  <CRow xs={{ gutterY: 3 }}>
                    {CreateFields().createInputSelectHash(
                      0,
                      "8",
                      "Campaign Type",
                      "campaign_type",
                      "campaign_type",
                      true,
                      "Choose a campaign type",
                      handleCampaignChange,
                      campaignsApiData,
                      "campaign_name",
                      "campaign_name",
                      campaignType,
                    )}

                    {campaignsApiData !== undefined &&
                    campaignsApiData.length > 0
                      ? campaignsApiData.map((object, key) =>
                          object["campaign_name"] === campaignType &&
                          "information_help" in object ? (
                            <CCol key={key} md={1}>
                              <CPopover
                                content={object["information_help"]}
                                placement="right"
                              >
                                <div>
                                  <CIcon icon={cilInfo} className="me-2" />
                                </div>
                              </CPopover>
                            </CCol>
                          ) : null
                        )
                      : null}

                    {CreateFields().createInputText(
                      1,
                      "8",
                      "Project Name",
                      "project_name",
                      "project_name",
                      true,
                      1,
                      20,
                      (head != null) ? (head["Project_Name"]):(""),
                      "",
                      null,
                      true,
                      "Project must have a name!"
                    )}

                    {CreateFields().createInputText(
                      2,
                      "8",
                      "Fault Injector path",
                      "fipath",
                      "fipath",
                      false,
                      1,
                      100,
                      (head != null) ? (head["Fault_Injector_Path"]):(""),
                      "",
                      null,
                      false,
                      "Project must have a fi!"
                    )}

                    {campaignsApiData !== undefined &&
                    campaignsApiData.length > 0
                      ? campaignsApiData.map((object, key) =>
                          object["campaign_name"] === campaignType &&
                          "configuration" in object &&
                          Object.entries(object["configuration"]).length > 0
                            ? Object.entries(object["configuration"]).map(
                                (folder, index) => (
                                  <div key={index} className="mb-3">
                                    <CRow className="justify-content-between">
                                      <CCol>
                                        <CFormLabel htmlFor="formFile">
                                          {"name" in folder[1] ? (
                                            <h6>{folder[1]["name"]}</h6>
                                          ) : (
                                            <h6>{folder[0]}</h6>
                                          )}
                                        </CFormLabel>
                                      </CCol>
                                      <CCol className="text-end">
                                        {"information_help" in folder[1] ? (
                                          <CPopover
                                            content={
                                              folder[1]["information_help"]
                                            }
                                            placement="right"
                                          >
                                            <div>
                                              <CIcon
                                                icon={cilInfo}
                                                className="me-2"
                                              />
                                            </div>
                                          </CPopover>
                                        ) : null}
                                      </CCol>
                                    </CRow>
                                    {(head==null)? (
                                      <CFormInput
                                        type="file"
                                        name={folder[0]}
                                        id={folder[0]}
                                        accept=".zip"
                                      />
                                    ) : null}
                                    {CreateFields().createInputText(
                                      2,
                                      "12",
                                      "",
                                      folder[0],
                                      folder[0],
                                      false,
                                      1,
                                      100,
                                      "",
                                      (head != null) ? (`File Copied from campaign with id: ${head['id']}`):("File path for " + folder[0] + " (case user do not upload file)"),
                                      null,
                                      false,
                                      "Project must have a file path!",
                                      (head != null) ? (true):(false)
                                    )}
                                  </div>
                                )
                              )
                            : null
                        )
                      : null}
                  </CRow>
                </CCardBody>
              </CCard>
            </CCol>

            <CCol sm={5}>
              <CCard>
                <CCardHeader className="text-center">
                  <h5>Parameters</h5>
                </CCardHeader>
                <CCardBody>
                  <CampaignParameters
                    campaignData={function_campaignsApiData(campaignType,campaignsApiData)}
                    campaignType={campaignType}
                  />
                </CCardBody>
              </CCard>
            </CCol>
          </CRow>
          <input type='hidden' id='Cloned_Campaign' name='Cloned_Campaign' value={(head!=null) ? (head['id']) : (-1) }></input>
          <CCol xs={12} className="text-center">
            <CButton color="primary" type="submit">
              Create campaign
            </CButton>
          </CCol>
        </CForm>
      </CContainer>
    </>
  );
};

export default CampaignSetup;
