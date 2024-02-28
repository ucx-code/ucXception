import React, { useState, useContext, useEffect } from "react";
import { useNavigate, useLocation } from "react-router-dom";
import AuthContext from "../../context/AuthProvider";
import AlertsContext from "../../context/AlertsProvider";
import API_Generic from "../../utils/api/API_Generic";
import {
  CButton,
  CCard,
  CCardBody,
  CCol,
  CContainer,
  CRow,
  CAlert,
  CTable,
  CTableHead,
  CTableRow,
  CTableHeaderCell,
  CTableBody,
  CTableDataCell,
  CPagination,
  CPaginationItem,
  CInputGroup,
  CFormInput,
  CFormSelect,
} from "@coreui/react";
import CIcon from "@coreui/icons-react";
import { cilWarning, cilChart, cilMagnifyingGlass } from "@coreui/icons";
import "@coreui/coreui/dist/css/coreui.min.css";

const Menu = () => {
  //Authenticaton
  const { isAuthenticated, handleLogout } = useContext(AuthContext);
  const { alert, addAlert, setAlert } = useContext(AlertsContext);
  const token = localStorage.getItem("token");
  const [logout, setLogout] = useState(false);

  //Nagivation
  const navigate = useNavigate();
  const location = useLocation();
  const from_login = location.state?.from?.pathname || "/login";

  const [campaignsMenu, setCampaignsMenu] = useState();
  const [totalCount, setTotalCount] = useState(0);
  const [page, setPage] = useState(1);

  const [itemsPage, setItemsPage] = useState(() => { 
    const storedPageSize = localStorage.getItem('pageSize');
    return storedPageSize ? parseInt(storedPageSize, 10) : 5;
  });

  useEffect(() => {                                     // Passed number of items to local Storage
    localStorage.setItem('pageSize', itemsPage.toString());
  }, [itemsPage]);

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

    API_Generic(setLogout, addAlert).genericCall(
      "/campaigns" + "?page=" + "1" + "&page_size=" + itemsPage,
      API_Generic(setLogout, addAlert).requestOptions("GET", token, null),
      false,
      setCampaignsMenu,
      null,
      null,
      getHeaders
    );
  }, []);

  const getHeaders = (headers) => {
    setTotalCount(parseInt(headers.get("X-Total-Count")));
  };

  const handleChangePage = (path, e) => {
    setAlert({});
    navigate(path);
  };

  const handlePreviousPage = () => {
    setPage(page - 1);

    filterCallAPI(
      document.getElementById("searchbar").value,
      page - 1,
      itemsPage
    );
  };

  const handleNextPage = () => {
    setPage(page + 1);

    filterCallAPI(
      document.getElementById("searchbar").value,
      page + 1,
      itemsPage
    );
  };

  const handleChangePageSize = (value) => {
    if (value) {
      setItemsPage(value);

      let aux = 0;
      if (page * parseInt(value) > totalCount) {
        aux = Math.ceil(totalCount / value);
        setPage(aux);
      } else {
        aux = page;
      }

      filterCallAPI(
        document.getElementById("searchbar").value,
        aux,
        value
      );
    }
  };

  const handleSearchBar = () => {
    filterCallAPI(document.getElementById("searchbar").value, page, itemsPage);
  };

  const handleKeyPress = (e) => {
    if (e.key === "Enter") {
      filterCallAPI(
        document.getElementById("searchbar").value,
        page,
        itemsPage
      );
    }
  };

  function filterCallAPI(searchbartext, page, pagesize) {
    API_Generic(setLogout, addAlert).genericCall(
      create_url(searchbartext, page, pagesize),
      API_Generic(setLogout, addAlert).requestOptions("GET", token, null),
      false,
      setCampaignsMenu,
      null,
      null,
      getHeaders
    );
  }

  function create_url(searchbartext, page, pagesize) {
    let url =
      "/campaigns" +
      "?page=" +
      page +
      "&page_size=" +
      pagesize +
      "&searchbar=" +
      searchbartext;

    return url;
  }

  return (
    <>
      <CContainer>
        <CRow className="justify-content-evenly">
          {Object.keys(alert).length !== 0 ? (
            <CAlert
              color={alert["color"]}
              className="d-flex align-items-center"
            >
              <CIcon
                icon={alert["icon"]}
                className="flex-shrink-0 me-2"
                width={24}
                height={24}
              />
              <div>{alert["message"]}</div>
            </CAlert>
          ) : null}
          <CCol sm={12}>
            <CCard>
              <CCardBody>
                <CRow>
                  <CCol xs={8} sm={4} md={6} lg={7} xl={8}>
                    <CButton
                      color="primary"
                      onClick={(e) => handleChangePage("/campaign/setup", e)}
                    >
                      New campaign
                    </CButton>
                  </CCol>
                  <CCol xs={4} sm={3} md={2} lg={2} xl={1}>
                    <CFormSelect
                      aria-label="Page"
                      onChange={(e) => handleChangePageSize(e.target.value)}
                      options={[
                        { label: "5", value: "5" },
                        { label: "10", value: "10" },
                        { label: "25", value: "25" },
                      ]}
                      value = {itemsPage}
                    />
                  </CCol>
                  <CCol xs={12} sm={5} md={4} lg={3} xl={3}>
                    <CInputGroup className="mb-3">
                      <CFormInput
                        id="searchbar"
                        name="searchbar"
                        placeholder="Search"
                        aria-label="Search"
                        aria-describedby="basic-addon2"
                        onKeyUp={(e) => handleKeyPress(e)}
                      />
                      <CButton onClick={handleSearchBar}>
                        <CIcon icon={cilMagnifyingGlass} size="lg" />
                      </CButton>
                    </CInputGroup>
                  </CCol>
                </CRow>

                <CRow>
                  <CCol className="text-center">
                    {campaignsMenu !== undefined &&
                    campaignsMenu !== null &&
                    Object.keys(campaignsMenu).length !== 0 ? (
                      <CTable striped responsive hover>
                        <CTableHead>
                          <CTableRow>
                            <CTableHeaderCell scope="col">
                              <h5>Campaign name</h5>
                            </CTableHeaderCell>
                            <CTableHeaderCell scope="col">
                              <h5>Execution name</h5>
                            </CTableHeaderCell>
                            <CTableHeaderCell scope="col">
                              <h5>Run type</h5>
                            </CTableHeaderCell>
                            <CTableHeaderCell scope="col">
                              <h5>Number of runs</h5>
                            </CTableHeaderCell>
                            <CTableHeaderCell scope="col">
                              <h5>Campaign type</h5>
                            </CTableHeaderCell>
                            <CTableHeaderCell scope="col">
                              <h5>Execution</h5>
                            </CTableHeaderCell>
                            <CTableHeaderCell scope="col">
                              <h5>Start date</h5>
                            </CTableHeaderCell>
                            <CTableHeaderCell scope="col"></CTableHeaderCell>
                          </CTableRow>
                        </CTableHead>
                        <CTableBody>
                          {campaignsMenu.map((object, key) => (
                            <CTableRow key={key}>
                              <CTableHeaderCell>
                                {object["campaignName"]}
                              </CTableHeaderCell>
                              <CTableHeaderCell>
                                {object["executionName"]}
                              </CTableHeaderCell>
                              <CTableHeaderCell>
                                {object["hasfault"] === 1 ? (
                                  <p className="fw-normal">Fault run</p>
                                ) : (
                                  <p className="fw-normal">Golden run</p>
                                )}
                              </CTableHeaderCell>
                              <CTableDataCell>
                                {object["ncurrentruns"]}/{object["ntargetruns"]}
                              </CTableDataCell>
                              <CTableDataCell>{object["type"]}</CTableDataCell>
                              <CTableDataCell>
                                {object["state"].charAt(0).toUpperCase() +
                                  object["state"].slice(1)}
                              </CTableDataCell>
                              <CTableDataCell>
                                {object["startdate"] ? (
                                  object["startdate"]
                                ) : (
                                  <p>Not started yet</p>
                                )}
                              </CTableDataCell>
                              <CTableDataCell>
                                {object["state"] === "ended" ? (
                                  <CButton
                                    color="info"
                                    variant="ghost"
                                    onClick={(e) =>
                                      handleChangePage(
                                        "/campaign/" +
                                          object["idcampaign"] +
                                          "/statistics",
                                        e
                                      )
                                    }
                                  >
                                    <CIcon icon={cilChart} size="lg" />
                                  </CButton>
                                ) : null}
                              </CTableDataCell>
                            </CTableRow>
                          ))}
                        </CTableBody>
                      </CTable>
                    ) : (
                      <h3>No campaigns created</h3>
                    )}
                  </CCol>
                </CRow>
                {campaignsMenu !== undefined && campaignsMenu.length !== 0 ? (
                  <CRow>
                    <CPagination
                      align="center"
                      aria-label="Page navigation example"
                    >
                      <CPaginationItem
                        disabled={page === 1 ? true : false}
                        onClick={handlePreviousPage}
                      >
                        Previous
                      </CPaginationItem>

                      <CPaginationItem
                        disabled={page * itemsPage >= totalCount ? true : false}
                        onClick={handleNextPage}
                      >
                        Next
                      </CPaginationItem>
                    </CPagination>
                  </CRow>
                ) : null}
              </CCardBody>
            </CCard>
          </CCol>
        </CRow>
      </CContainer>
    </>
  );
};

export default Menu;
