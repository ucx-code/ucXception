import { useState, useContext, useEffect } from "react";
import { useNavigate, useLocation, Link } from "react-router-dom";
import AuthContext from "../../context/AuthProvider";
import API_Generic from "../../utils/api/API_Generic";
import AlertsContext from "../../context/AlertsProvider";
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
  CFormLabel,
} from "@coreui/react";
import CIcon from "@coreui/icons-react";
import { cilWarning } from "@coreui/icons";
import "@coreui/coreui/dist/css/coreui.min.css";

const Login = () => {
  const { saveLogin, isAuthenticated } = useContext(AuthContext);
  const { alert, addAlert } = useContext(AlertsContext);

  const navigate = useNavigate();
  const location = useLocation();
  const from = location.state?.from?.pathname || "/menu";

  const [validated, setValidated] = useState(false);

  useEffect(() => {
    if (isAuthenticated()) {
      addAlert("Already logged in!", "warning", cilWarning);
      navigate(from, { replace: true });
    }
  }, []);

  const Prefunction = (values) => {
    saveLogin(values["user"]);
  };

  const Postfunction = () => {
    navigate(from, { replace: true });
  };

  const handleSubmit = (event) => {
    const form = event.currentTarget;
    event.preventDefault();
    if (form.checkValidity() === false) {
      event.stopPropagation();
    } else {
      const data = new FormData(event.target);
      const userinfo = {
        email: data.get("email_field"),
        password: data.get("password_field"),
      };

      API_Generic(null, addAlert).genericCall(
        "/login",
        API_Generic().requestOptions("POST", null, userinfo),
        true,
        null,
        Prefunction,
        Postfunction
      );
    }
    setValidated(true);
  };

  return (
    <>
      <div className="bg-light min-vh-100 d-flex flex-row align-items-center">
        <CContainer className="overflow-hidden">
          <CRow className="justify-content-center" xs={{ gutterY: 10 }}>
            <CCol md={12} lg={12} xl={12} className="text-center">
              <h1>ucXception</h1>
            </CCol>
            <CCol md={9} lg={8} xl={6}>
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
              <CCard className="p-4">
                <CCardBody>
                  <h1 className="text-center">Sign in</h1>
                  <CForm
                    className="row g-2 needs-validation"
                    noValidate
                    validated={validated}
                    onSubmit={handleSubmit}
                  >
                    <CCol md={12}>
                      <CFormLabel htmlFor="validationCustom01">
                        <h6>Email Adress</h6>
                      </CFormLabel>
                      <CFormInput
                        type="email"
                        placeholder="exemple@email.com"
                        id="validationCustom01"
                        name="email_field"
                        required
                      />
                      <CFormFeedback invalid>Must insert email!</CFormFeedback>
                    </CCol>

                    <CCol md={12}>
                      <CFormLabel htmlFor="validationCustom01">
                        <h6>Password</h6>
                      </CFormLabel>
                      <CFormInput
                        type="password"
                        placeholder="*********"
                        id="validationCustom02"
                        name="password_field"
                        required
                      />
                      <CFormFeedback invalid>
                        Must insert password!
                      </CFormFeedback>
                    </CCol>
                    <p />
                    <CCol xs={12} className="text-center">
                      <CButton color="primary" type="submit">
                        Sign In
                      </CButton>
                    </CCol>

                    <CCol xs={12} className="text-center">
                      New in ucXception? <Link to="/register">Sign up</Link>
                    </CCol>

                    <CCol xs={12} className="text-center">
                      <Link to="/reset">Forgot password</Link>
                    </CCol>
                  </CForm>
                </CCardBody>
              </CCard>
            </CCol>
          </CRow>
        </CContainer>
      </div>
    </>
  );
};

export default Login;
