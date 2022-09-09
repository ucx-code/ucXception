import { useState, useContext, useEffect } from "react";
import { Link, useNavigate, useLocation } from "react-router-dom";
import AuthContext from "../../context/AuthProvider";
import AlertsContext from "../../context/AlertsProvider";
import API_Generic from "../../utils/api/API_Generic";
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
import { cilLockLocked, cilUser, cilWarning } from "@coreui/icons";
import "@coreui/coreui/dist/css/coreui.min.css";

const Register = () => {
  const { isAuthenticated } = useContext(AuthContext);
  const { alert, addAlert, setAlert } = useContext(AlertsContext);

  const navigate = useNavigate();
  const location = useLocation();
  const from = location.state?.from?.pathname || "/login";

  const [validated, setValidated] = useState(false);

  useEffect(() => {
    setAlert({});
    if (isAuthenticated()) {
      addAlert("Already logged in!", "warning", cilWarning);
      navigate(from, { replace: true });
    }
  }, []);

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
        username: data.get("username_field"),
        email: data.get("email_field"),
        password: data.get("password_field"),
        repassword: data.get("repassword_field"),
      };

      API_Generic(null, addAlert).genericCall(
        "/register",
        API_Generic().requestOptions("POST", null, userinfo),
        true,
        null,
        null,
        Postfunction
      );
    }

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
                  <h1 className="text-center">Sign up</h1>
                  <p className="text-medium-emphasis text-center">
                    Create your account
                  </p>
                  <CInputGroup className="mb-3">
                    <CInputGroupText>
                      <CIcon icon={cilUser} />
                    </CInputGroupText>
                    <CFormInput
                      placeholder="Username"
                      type="text"
                      id="validationCustom01"
                      name="username_field"
                      required
                    />
                    <CFormFeedback invalid>Must insert username!</CFormFeedback>
                  </CInputGroup>

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

                  <CInputGroup className="mb-3">
                    <CInputGroupText>
                      <CIcon icon={cilLockLocked} />
                    </CInputGroupText>
                    <CFormInput
                      type="password"
                      placeholder="Password"
                      id="validationCustom03"
                      name="password_field"
                      required
                    />
                    <CFormFeedback invalid>
                      Must insert a password!
                    </CFormFeedback>
                  </CInputGroup>

                  <CInputGroup className="mb-4">
                    <CInputGroupText>
                      <CIcon icon={cilLockLocked} />
                    </CInputGroupText>
                    <CFormInput
                      type="password"
                      placeholder="Repeat password"
                      id="validationCustom04"
                      name="repassword_field"
                      required
                    />
                    <CFormFeedback invalid>
                      Must insert re-password!
                    </CFormFeedback>
                  </CInputGroup>

                  <CCol className="d-grid gap-2 col-3 mx-auto">
                    <CButton color="primary" type="submit">
                      Create
                    </CButton>
                  </CCol>
                </CForm>
                <p />
                <CCol className="text-center">
                  Already has an account? <Link to="/login">Login</Link>
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
