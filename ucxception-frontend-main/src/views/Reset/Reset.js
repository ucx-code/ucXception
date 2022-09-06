import { useState, useContext, useEffect } from "react";
import { Link, useNavigate, useLocation } from "react-router-dom";
import AuthContext from "../../context/AuthProvider";
import AlertsContext from "../../context/AlertsProvider";
import APIauxiliary from "../../utils/api/API_Generic";
import {
  CAlert,
  CButton,
  CCard,
  CCardBody,
  CCol,
  CContainer,
  CForm,
  CFormInput,
  CRow,
  CFormFeedback,
  CInputGroup,
  CInputGroupText,
} from "@coreui/react";
import CIcon from "@coreui/icons-react";
import { cilWarning, cilBurn, cilCheckCircle } from "@coreui/icons";
import "@coreui/coreui/dist/css/coreui.min.css";

const Register = () => {
  const { isAuthenticated } = useContext(AuthContext);
  const { alert, addAlert } = useContext(AlertsContext);

  const navigate = useNavigate();
  const location = useLocation();
  const from = location.state?.from?.pathname || "/login";

  const [validated, setValidated] = useState(false);

  useEffect(() => {
    if (isAuthenticated()) {
      addAlert("Already logged in!", "warning", cilWarning);
      navigate(from, { replace: true });
    }
  });

  async function resetPassword(userinfo) {
    await fetch(
      process.env.REACT_APP_API_URL + "/reset",
      APIauxiliary().requestOptions("POST", null, userinfo)
    )
      .then((response) => {
        if (response.status === 200 || response.status === 202) {
          response.json().then((values) => {
            addAlert(values["message"], "success", cilCheckCircle);
            navigate(from, { replace: true });
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
    if (form.checkValidity() === false) {
      event.preventDefault();
      event.stopPropagation();
    } else {
      const data = new FormData(event.target);
      const userinfo = {
        email: data.get("email_field"),
      };
      resetPassword(userinfo);
    }
    event.preventDefault();
    event.stopPropagation();
    setValidated(true);
  };

  return (
    <div className="bg-light min-vh-100 d-flex flex-row align-items-center">
      <CContainer>
        <CRow className="justify-content-center">
          <CCol md={9} lg={7} xl={6}>
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
            <CCard className="mx-4">
              <CCardBody className="p-4">
                <CForm
                  className="row g-2 needs-validation"
                  noValidate
                  validated={validated}
                  onSubmit={handleSubmit}
                >
                  <h1 className="text-center">Password reset</h1>

                  <CInputGroup className="mb-3">
                    <CInputGroupText>@</CInputGroupText>
                    <CFormInput
                      placeholder="Email"
                      type="email"
                      id="validationCustom02"
                      name="email_field"
                      required
                    />
                    <CFormFeedback invalid>Must insert an email!</CFormFeedback>
                  </CInputGroup>

                  <CCol className="d-grid gap-2 col-3 mx-auto">
                    <CButton color="primary" type="submit">
                      Reset
                    </CButton>
                  </CCol>
                </CForm>
                <p />
                <CCol className="text-center">
                  After clicking Reset you will receive an email with a link to
                  change your password.
                </CCol>
                <p />
                <CCol className="text-center">
                  Already has an account? <Link to="/login">Login</Link>
                </CCol>
                <p />
                <CCol className="text-center">
                  New in ucXception? <Link to="/register">Sign up</Link>
                </CCol>
              </CCardBody>
            </CCard>
          </CCol>
        </CRow>
      </CContainer>
    </div>
  );
};

export default Register;
