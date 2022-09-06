import React, { useState, useContext, useEffect } from "react";
import { useNavigate, useLocation } from "react-router-dom";
import AuthContext from "../../context/AuthProvider";
import AlertsContext from "../../context/AlertsProvider";
import API_Component from "../../utils/api/API_Component";
import API_Generic from "../../utils/api/API_Generic";
import ComponentParameters from "./ComponentParameters";
import AppBreadcrumb from "../../components/AppBreadcrumb";
import AppAlert from "../../components/AppAlerts";
import routes from "../../routes";
import {
  CButton,
  CCard,
  CCardBody,
  CCol,
  CContainer,
  CCardHeader,
  CForm,
  CRow,
  CListGroup,
  CListGroupItem,
  CFormCheck,
  CPopover,
} from "@coreui/react";
import CIcon from "@coreui/icons-react";
import { cilWarning, cilBurn, cilCheckCircle, cilInfo } from "@coreui/icons";
import "@coreui/coreui/dist/css/coreui.min.css";

const ComponentPage = () => {
  //Authenticaton
  const { isAuthenticated, handleLogout } = useContext(AuthContext);
  const { addAlert, setAlert } = useContext(AlertsContext);
  const token = localStorage.getItem("token");
  const [logout, setLogout] = useState(false);

  //Nagivation
  const navigate = useNavigate();
  const location = useLocation();
  const currentLocationPath = location.pathname;
  const currentLocation = location.pathname.replace("/", "");
  const from_menu = location.state?.from?.pathname || "/menu";
  const from_login = location.state?.from?.pathname || "/login";

  //Valdiate form
  const [validated, setValidated] = useState(false);

  //Components information API and input
  const [componentsData, setComponentsData] = useState({ data: [] });
  const [componentsNames, setComponentsNames] = useState();
  const [choosedComponent, setChoosedComponent] = useState();
  const [associatedComponents, setAssociatedComponents] = useState();
  const [component, setComponent] = useState();
  const [hosts, setHosts] = useState();

  const [createButon, setCreateButon] = useState(false);
  //User component data
  const [userComponents, setUserComponents] = useState();

  //Campaign ID
  const [campaignId, setCampaignId] = useState();

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
      //Get user components
      API_Component(setLogout, addAlert).getUserComponents(
        location.state["campaign_id"],
        token,
        null,
        setUserComponents,
        null,
        null
      );

      //Get components names
      API_Component(setLogout, addAlert).getComponentsNames(
        currentLocation,
        token,
        null,
        setComponentsNames,
        null,
        null
      );

      //Get user hosts
      API_Component(setLogout, addAlert).getUserHosts(
        location.state["campaign_id"],
        token,
        null,
        setHosts,
        changeDicioHosts,
        null
      );
    }
  }, []);

  //Handle change probe
  const handleComponent = (e) => {
    setAlert({});
    setComponent(e.target.value);

    let isEqual = false;
    for (let index = 0; index < componentsData["data"].length; index++) {
      if (componentsData["data"][index]["component_name"] === e.target.value) {
        isEqual = true;
        setChoosedComponent(componentsData["data"][index]);

        if ("components_allowed" in componentsData["data"][index]) {
          setCreateButon(true);
        }
      }
    }

    if (isEqual === false) {
      getComponentsData(e.target.value);
    }
  };

  //Change page
  const handleChangePage = (direction) => {
    let path;
    for (let i = 0; i < routes.length; i++) {
      if (routes[i]["pathname"] === currentLocationPath) {
        if (routes[i]["pathname"] !== undefined) {
          if (direction === "next") {
            path = location.state?.from?.pathname || routes[i + 1]["pathname"];
          } else if (direction === "previous") {
            path = location.state?.from?.pathname || routes[i - 1]["pathname"];
          }
        } else {
          path = location.state?.from?.pathname || routes[i]["pathname"];
        }
      }
    }
    setAlert({});
    navigate(path, {
      state: { campaign_id: location.state["campaign_id"] },
      replace: true,
    });
    window.location.reload(false);
  };

  const handleExecute = () => {
    const body = {
      campaign_id: location.state["campaign_id"],
    };

    API_Component(setLogout, addAlert).executeCampaign(
      token,
      body,
      null,
      null,
      null
    );

    navigate("/menu");
  };

  const changeDicioHosts = (values) => {
    let new_hosts = [];
    for (let index = 0; index < values.length; index++) {
      let label;
      if (values[index].domain !== null) {
        label = values[index].domain;
      } else {
        label = values[index].type;
      }

      new_hosts.push({
        value: values[index].idhost,
        label: label,
      });
    }
    return new_hosts;
  };

  //Get components data
  function getComponentsData(target) {
    fetch(
      process.env.REACT_APP_API_URL +
        "/components/" +
        currentLocation +
        "/" +
        target,
      API_Generic().requestOptions("GET", token, null)
    ).then((response) => {
      if (response.status === 200) {
        response.json().then((values) => {
          setChoosedComponent(values);

          setComponentsData((prevState) => ({
            data: [...prevState.data, values],
          }));

          if ("components_allowed" in values) {
            getComponentAllowed(target);
          } else {
            setCreateButon(true);
          }
        });
      } else if (response.status === 401) {
        response.json().then((values) => {
          addAlert(values["message"], "danger", cilBurn);
          handleLogout();
          navigate(from_login, { replace: true });
        });
      } else {
        response.json().then((values) => {
          setCreateButon(false);
          addAlert(values["message"], "danger", cilBurn);
        });
      }
    });
  }

  //Get components data
  function getComponentAllowed(target) {
    fetch(
      process.env.REACT_APP_API_URL +
        "/components/" +
        currentLocation +
        "/" +
        target +
        "/" +
        campaignId,
      API_Generic().requestOptions("GET", token, null)
    ).then((response) => {
      if (response.status === 200) {
        response.json().then((values) => {
          setAssociatedComponents(values);

          setCreateButon(true);
        });
      } else if (response.status === 401) {
        response.json().then((values) => {
          addAlert(values["message"], "danger", cilBurn);
          handleLogout();
          navigate(from_login, { replace: true });
        });
      } else {
        response.json().then((values) => {
          setCreateButon(false);
          addAlert(values["message"], "danger", cilBurn);
        });
      }
    });
  }

  function createComponent(componentInformation) {
    const requestOptions = {
      method: "POST",
      headers: { Authentication: token },
      body: componentInformation,
    };

    fetch(process.env.REACT_APP_API_URL + "/components", requestOptions)
      .then((response) => {
        if (response.status === 200) {
          response.json().then((values) => {
            addAlert(values["message"], "success", cilCheckCircle);
            window.location.reload(false);
          });
        } else if (response.status === 401) {
          response.json().then((values) => {
            addAlert(values["message"], "danger", cilBurn);
            handleLogout();
            navigate(from_login, { replace: true });
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

  const handleSubmit = (event) => {
    const form = event.currentTarget;
    event.preventDefault();
    if (form.checkValidity() === false) {
      event.stopPropagation();
    } else {
      const data = new FormData(form);
      data.set("component_type", currentLocation);
      data.set("campaign_id", campaignId);

      //Iterate component data
      for (let i = 0; i < componentsData["data"].length; i++) {
        //Search for selected component type
        if (
          componentsData["data"][i]["component_name"] ===
          data.get("componentType")
        ) {
          //Get parameters from component type choosed
          Object.keys(componentsData["data"][i]["parameters"]).forEach(
            function (key) {
              let aux;

              if (
                componentsData["data"][i]["parameters"][key]["type"] ===
                "multiple"
              ) {
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
                  if (componentsData["data"][i]["parameters"][key]["default"]) {
                    data.append(
                      key,
                      componentsData["data"][i]["parameters"][key]["default"]
                    );
                  } else {
                    data.append(key, false);
                  }
                }
              }
            }
          );
        }
      }
      for (var pair of data.entries()) {
        console.log(pair[0] + ", " + pair[1]);
      }
      createComponent(data);
    }
    setValidated(true);
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
          <CRow>
            <AppAlert />
            <AppBreadcrumb />
          </CRow>

          <CRow xs={{ gutterY: 3 }}>
            <CCol sm={6} md={6} lg={3}>
              <CCard>
                <CCardHeader className="text-center">
                  <h5>User components</h5>
                </CCardHeader>
                <CListGroup flush>
                  {userComponents !== undefined && userComponents.length > 0 ? (
                    userComponents.map((component, index) => {
                      return (
                        <CListGroupItem key={index} className="text-center">
                          <h6>{component["name"]}</h6>({component["type"]})
                        </CListGroupItem>
                      );
                    })
                  ) : (
                    <CListGroupItem className="text-center">
                      <h6>No user components!</h6>
                    </CListGroupItem>
                  )}
                </CListGroup>
              </CCard>
            </CCol>

            <CCol sm={6} md={6} lg={4}>
              <CCard>
                <CCardHeader className="text-center">
                  <h5>Components</h5>
                </CCardHeader>
                <CListGroup flush>
                  {componentsNames !== undefined
                    ? componentsNames.map((component, index) => {
                        return (
                          <CListGroupItem key={index}>
                            <CFormCheck
                              type="radio"
                              name="component_name"
                              id={"component_name" + index}
                              value={component}
                              label={component}
                              onClick={(e) => handleComponent(e)}
                            />
                          </CListGroupItem>
                        );
                      })
                    : null}
                </CListGroup>
              </CCard>
            </CCol>

            <CCol sm={12} md={12} lg={5}>
              <CCard>
                <CCardHeader className="text-center">
                  {component === undefined ? (
                    <h5>Component Name</h5>
                  ) : (
                    <>
                      {choosedComponent !== undefined &&
                      "information_help" in choosedComponent ? (
                        <CPopover
                          content={choosedComponent["information_help"]}
                          placement="bottom"
                        >
                          <h5>
                            {component}{" "}
                            <CIcon icon={cilInfo} className="me-2" />
                          </h5>
                        </CPopover>
                      ) : null}
                    </>
                  )}
                </CCardHeader>
                <CCardBody>
                  {choosedComponent !== undefined &&
                  choosedComponent !== null ? (
                    createButon !== undefined && createButon === true ? (
                      <ComponentParameters
                        componentData={choosedComponent}
                        associatedComponents={associatedComponents}
                        hosts={hosts}
                      />
                    ) : (
                      <CCol className="text-center" sm={12}>
                        <h6>
                          Make sure you have created the necessary components to
                          use this probe!
                        </h6>
                      </CCol>
                    )
                  ) : (
                    <CCol className="text-center" sm={12}>
                      <h6>First need to choose a component</h6>
                    </CCol>
                  )}
                </CCardBody>
              </CCard>
            </CCol>

            <CCol sm={6} md={8} lg={10}>
              <CButton
                color="primary"
                onClick={(e) => handleChangePage("previous")}
              >
                Back
              </CButton>
            </CCol>

            {routes !== undefined && currentLocationPath !== undefined ? (
              routes[routes.length - 1]["pathname"] !== currentLocationPath ? (
                <>
                  <CCol sm={3} md={2} lg={1}>
                    <CButton color="primary" onClick={handleExecute}>
                      Execute
                    </CButton>
                  </CCol>

                  <CCol sm={3} md={2} lg={1}>
                    <CButton
                      color="primary"
                      onClick={(e) => handleChangePage("next")}
                    >
                      Next
                    </CButton>
                  </CCol>
                </>
              ) : (
                <>
                  <CCol sm={3} md={2} lg={1}></CCol>
                  <CCol sm={3} md={2} lg={1}>
                    <CButton color="primary" onClick={handleExecute}>
                      Execute
                    </CButton>
                  </CCol>
                </>
              )
            ) : null}
          </CRow>
        </CForm>
      </CContainer>
    </>
  );
};

export default ComponentPage;
