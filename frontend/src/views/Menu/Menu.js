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
  CSpinner,
  CAccordionItem,
  CAccordionHeader,
  CAccordionBody,
  CAccordion,
} from "@coreui/react";
import CIcon from "@coreui/icons-react";
import { cilWarning, cilChart, cilMagnifyingGlass, cilTrash } from "@coreui/icons";
import "@coreui/coreui/dist/css/coreui.min.css";
import API_Campaign from "../../utils/api/API_Campaign";

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

  // Passes the number of items displayed to local Storage
  useEffect(() => {
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
    handleGetCampaignsList();
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
      sort_array,
      null,
      null,
      getHeaders
    );
  }

  // Function that arranges the array with types of runs that have the same campaign id
  const sort_array =(campaignsMenu) => {
    let novo_array = [], novo_index = 0;
    for (let index = 0; index < campaignsMenu.length - 1; index++) {
      novo_array[novo_index] = { "campaignName" : campaignsMenu[index]["campaignName"] };
      if(campaignsMenu[index]["idcampaign"] === campaignsMenu[index+1]["idcampaign"]){
        if (campaignsMenu[index]["hasfault"] === 0 && campaignsMenu[index+1]["hasfault"] === 1){
          novo_array[novo_index++]["type"] = {"golden":campaignsMenu[index], "faulty": campaignsMenu[index+1]};
          index++;
        }
        else{
          novo_array[novo_index++]["type"] = {"golden":campaignsMenu[index+1], "faulty": campaignsMenu[index]};
          index++;          
        }
      }
      else{
        if (campaignsMenu[index]["hasfault"] === 1){
          novo_array[novo_index++]["type"] = {"faulty":campaignsMenu[index]};
        }
        else{
          novo_array[novo_index++]["type"] = {"golden":campaignsMenu[index]};
        }
      }
    }
    setCampaignsMenu(novo_array);  
  }

  // Handles the campaigns displayed
  const handleGetCampaignsList =() =>  {
  API_Generic(setLogout, addAlert).genericCall(
    "/campaigns" + "?page=" + "1" + "&page_size=" + itemsPage,
    API_Generic(setLogout, addAlert).requestOptions("GET", token, null),
    false,
    sort_array,
    null,
    null,
    getHeaders
  );
  }


  // Function will call api to delete Campaign
  const handleDeleteCampaign = (campaignId) => {
    API_Campaign(setLogout, addAlert).deleteCampaign(
      campaignId,
      token,
      null,
      null,
      handleGetCampaignsList
    );
  };

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

  // Function that renders a single row (if number = 2, it will render rows for accordion)
  function render(tipo,object,key,number){
    return(
    <CTableRow key={key}>
      {number === 2 ? ( 
        <CTableDataCell></CTableDataCell>
      ) : ( 
        <CTableHeaderCell>{object["type"][tipo]["campaignName"]}</CTableHeaderCell>
      )}
      <CTableDataCell>{object["type"][tipo]["executionName"]}</CTableDataCell>
      <CTableHeaderCell>
      {tipo === "faulty" ? (
        <p>Fault run</p>
      ) : (
        <p>Golden run</p>
      )}
      </CTableHeaderCell>
      <CTableDataCell>{object["type"][tipo]["ncurrentruns"]}/{object["type"][tipo]["ntargetruns"]}</CTableDataCell>
      {number === 2 ? ( 
        <CTableDataCell></CTableDataCell>
      ) : ( 
        <CTableHeaderCell>{object["type"][tipo]["type"]}</CTableHeaderCell>
      )}
      <CTableDataCell>{object["type"][tipo]["state"].charAt(0).toUpperCase() + object["type"][tipo]["state"].slice(1)}</CTableDataCell>
      {number === 2 ? ( 
        <CTableDataCell></CTableDataCell>
      ) : ( 
        <CTableDataCell>
        {object["type"][tipo]["startdate"] ? (
          object["type"][tipo]["startdate"]
        ) : (
          <p>Not started yet</p>
        )}
        </CTableDataCell>
      )}
      <CTableDataCell>
        {object["type"][tipo]["state"] === "ended" ? (
          <CButton
            color="info"
            variant="ghost"
            onClick={(e) => handleChangePage("/campaign/" + object["type"][tipo]["idcampaign"] + "/statistics", e)} >
            <CIcon icon={cilChart} size="lg" className="text-primary" />
          </CButton>
        ) : object["type"][tipo]["state"] === "ended with error" ? (                                
          <CButton
            color="danger"
            variant="ghost"
            onClick={(e) => handleDeleteCampaign(object["type"][tipo]["idcampaign"])} >
            <CIcon icon={cilTrash} size="lg" className="text-primary"/>
          </CButton>
        ) : object["type"][tipo]["state"] === "executing" ? (
          <CSpinner color="primary" />
        ) : null}
      </CTableDataCell>
    </CTableRow>
    )}

// Function that renders the dropdown (accordion) when the same campaign has golden and fault run
function render_accordion(object,key){
  return(
    <CTableRow>
      <CTableDataCell colSpan={8}>
        <CAccordion><style>{`.accordion-button::after{background-image:initial;}.accordion-button:not(.collapsed)::after{background-image:initial;}`}</style>
          <CAccordionItem>
            <CAccordionHeader>
              <CTable colSpan={8}>
                <div style={{ display: 'flex', justifyContent: 'space-between' }}>
                <CTableHeaderCell>{object["type"]["golden"]["campaignName"]}</CTableHeaderCell>
                <CTableDataCell></CTableDataCell>
                <CTableDataCell></CTableDataCell>
                <CTableDataCell></CTableDataCell>
                <CTableHeaderCell>{object["type"]["golden"]["type"]}</CTableHeaderCell>
                <CTableDataCell>
                  {object["type"]["golden"]["startdate"] ? (
                    object["type"]["golden"]["startdate"]
                  ) : (
                    <p>Not started yet</p>
                  )}
                </CTableDataCell>
                </div>
              </CTable>
            </CAccordionHeader>
            <CAccordionBody>
              <CTable>
                <CTableHead>
                  <CTableRow>
                    <CTableHeaderCell scope="col">
                      <h5 style={{ visibility: 'hidden' }}>Campaign name</h5>
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
                      <h5 style={{ visibility: 'hidden' }}>Campaign type</h5>
                    </CTableHeaderCell>
                    <CTableHeaderCell scope="col">
                      <h5>Execution</h5>
                    </CTableHeaderCell>
                    <CTableHeaderCell scope="col">
                      <h5 style={{ visibility: 'hidden' }}>Start date</h5>
                    </CTableHeaderCell>
                    <CTableHeaderCell scope="col">   
                    </CTableHeaderCell>
                  </CTableRow>
                </CTableHead>
                <CTableBody>
                {render("golden", object, key + "golden",2)}
                {render("faulty", object, key + "faulty",2)}
                </CTableBody>
              </CTable>
            </CAccordionBody>
          </CAccordionItem>
        </CAccordion>
      </CTableDataCell>
    </CTableRow>
  )}
  
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
                            <CTableHeaderCell scope="col">   
                            </CTableHeaderCell>
                          </CTableRow>
                        </CTableHead>
                        <CTableBody>
                          {campaignsMenu.map((object, key) => (
                            <>
                            {object["type"].hasOwnProperty("faulty") && object["type"].hasOwnProperty("golden") ? (render_accordion(object,key)) : 
                            object["type"].hasOwnProperty("golden") ? (render("golden",object,key,1)) : 
                            object["type"].hasOwnProperty("faulty") ? (render("faulty",object,key,1)) : null}
                            </>
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
