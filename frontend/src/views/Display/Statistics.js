import React, { useState, useContext, useEffect } from "react";
import { useNavigate, useLocation } from "react-router-dom";
import AuthContext from "../../context/AuthProvider";
import AlertsContext from "../../context/AlertsProvider";
import AppAlert from "../../components/AppAlerts";
import API_Display from "../../utils/api/API_Display";
import APIauxiliary from "../../utils/api/API_Generic";
import {
  CButton,
  CCard,
  CCardBody,
  CCol,
  CContainer,
  CCardHeader,
  CRow,
  CPopover,
  CTable,
  CTableHead,
  CTableRow,
  CTableHeaderCell,
  CTableBody,
  CTableDataCell,
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
  LineChart,
  Line,
  CartesianGrid,
  XAxis,
  YAxis,
  Tooltip,
  Legend,
  Label,
} from "recharts";

const CampaignSatistics = () => {
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
  const [statistics, setStatistics] = useState();
  const [campaign, setCampaign] = useState();
  const [message, setMessage] = useState();

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

    //Get statistics campaign data
    API_Display(setLogout, addAlert).getCampaignStatistics(
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
      if ("statistics" in values) {
        setStatistics(values["statistics"]);
      }
      if ("message" in values) {
        setMessage(values["message"]);
      }
    }
    return values;
  };

  const handleChangePage = () => {
    setAlert({});
    navigate("/campaign/" + currentLocation[1] + "/charts", { replace: true });
  };

  const handleDownloadFile = () => {
    API_Display(setLogout, addAlert).getDownloadFile(currentLocation[1], token);
  };

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
                      Charts
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
                  </CCol>
                </CRow>
              </CCardHeader>
              <CCardBody>
                <CRow>
                  {statistics !== null && statistics !== undefined ? (
                    <>
                      <CCol>
                      {statistics["golden_run_min_peak_duration"] ? (
                        <>
                        <CRow>
                          <h4>Golden Runs</h4>
                        </CRow>

                        <CTable responsive>
                          <CTableHead>
                            <CTableRow>
                              <CTableHeaderCell scope="col">
                                Peak Duration (s)
                              </CTableHeaderCell>
                              <CTableHeaderCell scope="col">
                                Mininum
                              </CTableHeaderCell>
                              <CTableHeaderCell scope="col">
                                Mean
                              </CTableHeaderCell>
                              <CTableHeaderCell scope="col">
                                Maximum
                              </CTableHeaderCell>
                              <CTableHeaderCell scope="col">
                                Total
                              </CTableHeaderCell>
                            </CTableRow>
                          </CTableHead>
                          <CTableBody>
                            <CTableRow>
                              <CTableDataCell scope="row"></CTableDataCell>
                              <CTableDataCell>
                                {statistics["golden_run_min_peak_duration"]}
                              </CTableDataCell>
                              <CTableDataCell>
                                {statistics["golden_run_mean_peak_duration"]}
                              </CTableDataCell>
                              <CTableDataCell>
                                {statistics["golden_run_max_peak_duration"]}
                              </CTableDataCell>
                              <CTableDataCell>
                                {statistics["golden_run_total_duration"]}
                              </CTableDataCell>
                            </CTableRow>
                          </CTableBody>
                        </CTable>
                        </>) : null}

                        {statistics["data_chart_crashes"] ? (  
                        <>
                        <CRow>
                          <h4>Fault Runs</h4>
                        </CRow>

                        <CTable responsive>
                          <CTableHead>
                            <CTableRow>
                              <CTableHeaderCell scope="col">
                                Peak Duration (s)
                              </CTableHeaderCell>
                              <CTableHeaderCell scope="col">
                                Mininum
                              </CTableHeaderCell>
                              <CTableHeaderCell scope="col">
                                Mean
                              </CTableHeaderCell>
                              <CTableHeaderCell scope="col">
                                Maximum
                              </CTableHeaderCell>
                              <CTableHeaderCell scope="col">
                                Total
                              </CTableHeaderCell>
                            </CTableRow>
                          </CTableHead>
                          <CTableBody>
                            <CTableRow>
                              <CTableDataCell scope="row"></CTableDataCell>
                              <CTableDataCell>
                                {statistics["fault_run_min_peak_duration"]}
                              </CTableDataCell>
                              <CTableDataCell>
                                {statistics["fault_run_mean_peak_duration"]}
                              </CTableDataCell>
                              <CTableDataCell>
                                {statistics["fault_run_max_peak_duration"]}
                              </CTableDataCell>
                              <CTableDataCell>
                                {statistics["fault_run_total_duration"]}
                              </CTableDataCell>
                            </CTableRow>
                          </CTableBody>
                        </CTable>

                        <CRow>
                          <h4>Failure modes</h4>
                        </CRow>

                        <CTable responsive>
                          <CTableHead>
                            <CTableRow>
                              <CTableHeaderCell scope="col">
                                Every type of crashes (%)
                              </CTableHeaderCell>
                              <CTableHeaderCell scope="col">
                                Watchdog fault runs (%)
                              </CTableHeaderCell>
                            </CTableRow>
                          </CTableHead>
                          <CTableBody>
                            <CTableRow>
                              <CTableDataCell>
                                {statistics["percentage_crashes"]}
                              </CTableDataCell>
                              <CTableDataCell>
                                {
                                  statistics[
                                    "percentage_watchdogs_fault_crashes"
                                  ]
                                }
                              </CTableDataCell>
                            </CTableRow>
                          </CTableBody>
                        </CTable>
                        </>) : null}
                        {"percentage_incorrect_output" in statistics ? (
                          <CTable responsive>
                            <CTableHead>
                              <CTableRow>
                                <CTableHeaderCell scope="col">
                                  Incorrect output
                                </CTableHeaderCell>
                                <CTableHeaderCell scope="col">
                                  Correct output
                                </CTableHeaderCell>
                              </CTableRow>
                            </CTableHead>
                            <CTableBody>
                              <CTableRow>
                                <CTableDataCell>
                                  {statistics["percentage_incorrect_output"]}
                                </CTableDataCell>
                              </CTableRow>
                            </CTableBody>
                          </CTable>
                        ) : null}
                      </CCol>
                      <CCol>
                        {statistics["data_chart_crashes"] ? (
                          <ResponsiveContainer width="100%" height={300}>
                            <LineChart data={statistics["data_chart_crashes"]}>
                              <CartesianGrid
                                stroke="#ccc"
                                strokeDasharray="5 5"
                              />
                              <XAxis dataKey="run_n">
                                <Label
                                  value="Number of runs"
                                  offset={0}
                                  position="insideBottom"
                                />
                              </XAxis>
                              <YAxis
                                label={{
                                  value: "Probability",
                                  angle: -90,
                                  position: "insideLeft",
                                }}
                              />
                              <Tooltip />
                              <Legend verticalAlign="top" height={36} />
                              <Line
                                type="monotone"
                                dataKey="all_type_crashes"
                                stroke="#8884d8"
                                dot={false}
                              />
                              <Line
                                type="monotone"
                                dataKey="watchdog_type_crashes"
                                stroke="#82ca9d"
                                dot={false}
                              />
                            </LineChart>
                          </ResponsiveContainer>
                        ) : (
                          <h5>
                            Chart will not be displayed because no data has been
                            processed.
                          </h5>
                        )}
                      </CCol>
                    </>
                  ) : (
                    <CCol>
                      <h5> Nothing to display related to this campaign.</h5>
                    </CCol>
                  )}
                </CRow>
              </CCardBody>
            </>
          ) : (
            <>
              <CCardHeader component="h5"> Campaign information</CCardHeader>
              <CCardBody>
                <h5> Nothing to display related to this campaign.</h5>
              </CCardBody>
            </>
          )}
        </CCard>
      </CContainer>
    </>
  );
};

export default CampaignSatistics;
