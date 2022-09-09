import React, { useState, useContext, useEffect } from "react";
import { useNavigate, useLocation } from "react-router-dom";
import AuthContext from "../../context/AuthProvider";
import AlertsContext from "../../context/AlertsProvider";
import AppAlert from "../../components/AppAlerts";
import API_Display from "../../utils/api/API_Display";
import CreateFields from "../../utils/CreateFields";
import {
  CButton,
  CCard,
  CCardBody,
  CCol,
  CContainer,
  CCardHeader,
  CRow,
  CPopover,
} from "@coreui/react";
import CIcon from "@coreui/icons-react";
import {
  cilWarning,
  cilBurn,
  cilDataTransferDown,
  cilTrash,
  cilTag,
} from "@coreui/icons";
import "@coreui/coreui/dist/css/coreui.min.css";
import {
  ResponsiveContainer,
  BarChart,
  Bar,
  LineChart,
  Line,
  CartesianGrid,
  XAxis,
  YAxis,
  Tooltip,
  Legend,
  Label,
} from "recharts";

const CampaignCharts = () => {
  //Authenticaton
  const { isAuthenticated, handleLogout } = useContext(AuthContext);
  const { addAlert, setAlert } = useContext(AlertsContext);
  const token = localStorage.getItem("token");
  const [logout, setLogout] = useState(false);

  //Nagivation
  const navigate = useNavigate();
  const location = useLocation();
  const currentLocation = location.pathname.replace("/", "").split("/");
  const from_login = location.state?.from?.pathname || "/login";

  //Hold campaign statistics
  const [chartdata, setChartData] = useState();
  const [columns, setColumns] = useState();
  const [campaign, setCampaign] = useState();

  const [xaxis, setXAxis] = useState();
  const [yaxis, setYAxis] = useState();
  const [chartType, setChartType] = useState("LineChart");

  useEffect(() => {
    if (logout === true) {
      handleLogout();
      navigate(from_login, { replace: true });
    }
  }, [logout]);

  useEffect(() => {
    if (!isAuthenticated()) {
      addAlert("Need to login first!", "warning", cilWarning);
      navigate(from_login);
    }
    //Set alerts to empty
    setAlert({});

    API_Display(setLogout, addAlert).getColumnsCsvfile(
      currentLocation[1],
      token,
      preFunction
    );
  }, []);

  const preFunction = (values) => {
    if (values !== undefined) {
      if ("campaign" in values) {
        setCampaign(values["campaign"]);
      }
      if ("columns" in values) {
        setXAxis(values["columns"][0]);
        setYAxis(values["columns"][0]);
        setColumns(values["columns"]);
        handleRequestChartData(values["columns"][0], values["columns"][0]);
      }
      if ("message" in values) {
        addAlert(values["message"], "danger", cilBurn);
      }
    }
    return values;
  };

  const handleChangePage = () => {
    setAlert({});
    navigate("/campaign/" + currentLocation[1] + "/statistics", {
      replace: true,
    });
  };

  const handleDownloadFile = () => {
    API_Display(setLogout, addAlert).getDownloadFile(currentLocation[1], token);
  };

  const handleRequestChartData = (xaxis, yaxis) => {
    const body = { x_axis: xaxis, y_axis: yaxis };

    API_Display(setLogout, addAlert).getCampaignCharts(
      currentLocation[1],
      token,
      body,
      setChartData
    );
  };

  const handleChangeAxis = (e) => {
    if (e.target.id === "x_axis") {
      setXAxis(e.target.value);
      handleRequestChartData(e.target.value, yaxis);
    } else if (e.target.id === "y_axis") {
      setYAxis(e.target.value);
      handleRequestChartData(xaxis, e.target.value);
    }
  };

  const handleChangeChartType = (e) => {
    setChartType(e.target.value);
  };

  function renderChart() {
    if (chartType === "LineChart") {
      return renderLineChart();
    } else if (chartType === "BarChart") {
      return renderBarChart();
    }
  }

  function renderLineChart() {
    return (
      <LineChart data={chartdata}>
        <CartesianGrid strokeDasharray="3 3" />
        <XAxis dataKey="xaxis_value">
          <Label value={xaxis} offset={0} position="insideBottom" />
        </XAxis>
        <YAxis
          label={{
            value: yaxis,
            angle: -90,
            position: "insideLeft",
          }}
        />
        <Tooltip />
        <Legend verticalAlign="top" height={36} />
        <Line type="monotone" dataKey="yaxis_value" stroke="#82ca9d" />
      </LineChart>
    );
  }

  function renderBarChart() {
    return (
      <BarChart data={chartdata}>
        <CartesianGrid strokeDasharray="3 3" />
        <XAxis dataKey="xaxis_value" />
        <YAxis />
        <Tooltip />
        <Legend />
        <Bar dataKey="yaxis_value" fill="#82ca9d" />
      </BarChart>
    );
  }

  return (
    <>
      <CContainer>
        <CRow>
          <AppAlert />
        </CRow>
        <CCard>
          {campaign !== null && campaign !== undefined ? (
            <>
              <CCardHeader component="h5">
                <CRow>
                  <CCol
                    xs="auto"
                    sm="auto"
                    md="auto"
                    lg="auto"
                    xl="auto"
                    className="me-auto"
                  >
                    <CPopover
                      content={
                        <>
                          <CRow>
                            <CCol>
                              <h6>Type:</h6>
                            </CCol>
                            <CCol>{campaign["type"]}</CCol>
                          </CRow>
                          <CRow>
                            <CCol>
                              <h6>State: </h6>
                            </CCol>
                            <CCol>
                              {campaign["state"].slice(0, 1).toUpperCase() +
                                campaign["state"].slice(
                                  1,
                                  campaign["state"].length
                                )}
                            </CCol>
                          </CRow>
                          <CRow>
                            <CCol>
                              <h6>Start date:</h6>
                            </CCol>
                            <CCol>
                              {campaign["startdate"] != null ? (
                                campaign["startdate"]
                              ) : (
                                <a> Not started </a>
                              )}
                            </CCol>
                          </CRow>
                          <CRow>
                            <CCol>
                              <h6>End date:</h6>
                            </CCol>
                            <CCol>
                              {campaign["enddate"] != null ? (
                                campaign["enddate"]
                              ) : (
                                <a> Not estimated </a>
                              )}
                            </CCol>
                          </CRow>
                        </>
                      }
                      placement="right"
                    >
                      <CButton color="dark" variant="ghost">
                        <h5>
                          {campaign["name"]} <CIcon icon={cilTag} size="sm" />
                        </h5>
                      </CButton>
                    </CPopover>
                  </CCol>

                  <CCol md="auto">
                    <CButton color="light" onClick={handleChangePage}>
                      Statistics
                    </CButton>
                  </CCol>

                  <CCol
                    xs="auto"
                    sm="auto"
                    md="auto"
                    lg="auto"
                    xl="auto"
                    className="text-end"
                  >
                    <CButton
                      color="info"
                      variant="ghost"
                      onClick={handleDownloadFile}
                    >
                      <CIcon icon={cilDataTransferDown} size="lg" />
                    </CButton>
                    {/* <CButton color="danger" variant="ghost">
                      <CIcon icon={cilTrash} size="lg" />
                    </CButton> */}
                  </CCol>
                </CRow>
              </CCardHeader>
              <CCardBody>
                <CRow>
                  <CCol sm={8} md={12} lg={5} xl={6}>
                    {CreateFields().createInputSelectList(
                      0,
                      "5",
                      "Chart type",
                      "chart_type",
                      "chart_type",
                      true,
                      "Choose chart type",
                      handleChangeChartType,
                      ["LineChart", "BarChart"]
                    )}

                    {CreateFields().createInputSelectList(
                      1,
                      "5",
                      "X Axis value",
                      "x_axis",
                      "x_axis",
                      true,
                      "Choose the value for X Axis",
                      handleChangeAxis,
                      columns
                    )}

                    {CreateFields().createInputSelectList(
                      2,
                      "5",
                      "Y Axis value",
                      "y_axis",
                      "y_axis",
                      true,
                      "Choose the value for Y Axis",
                      handleChangeAxis,
                      columns
                    )}

                    {/* <CButton color="light" onClick={handleRequestChartData}>
                      Create chart
                    </CButton> */}
                  </CCol>
                  <CCol sm={12} md={12} lg={7} xl={6}>
                    {chartdata !== undefined && chartdata !== null ? (
                      <ResponsiveContainer width="100%" height={300}>
                        {renderChart()}
                      </ResponsiveContainer>
                    ) : (
                      <h5>
                        Chart will not be displayed because no data has been
                        processed.
                      </h5>
                    )}
                  </CCol>
                </CRow>
              </CCardBody>
            </>
          ) : (
            <>
              <CCardHeader component="h5"> Campaign information</CCardHeader>
              <CCardBody>
                <h5> Nothing to display related to this campaign</h5>
              </CCardBody>
            </>
          )}
        </CCard>
      </CContainer>
    </>
  );
};

export default CampaignCharts;
