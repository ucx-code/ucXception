import React, { useContext } from "react";
import AuthContext from "../../context/AuthProvider";
import AlertsContext from "../../context/AlertsProvider";
import {
  CDropdown,
  CDropdownDivider,
  CDropdownHeader,
  CDropdownItem,
  CDropdownMenu,
  CDropdownToggle,
} from "@coreui/react";
import { cilAccountLogout, cilUser, cilCheckCircle, cilEnvelopeClosed, cilSettings } from "@coreui/icons";
import CIcon from "@coreui/icons-react";

export default function AppHeaderDropdown() {
  const { handleLogout } = useContext(AuthContext);
  const { addAlert } = useContext(AlertsContext);

  const handleLogoutButton = () => {
    handleLogout();
    addAlert("Success logout", "success", cilCheckCircle);
  };

  // Get from local Storage the name and the email of the logged in user
  const user = JSON.parse(localStorage.getItem('User'));

  return (
    <CDropdown variant="nav-item" alignment="end">
      <CDropdownToggle placement="bottom-end" className="py-0" caret={false}>
        <CIcon icon={cilSettings} size="lg" className="my-1 mx-2" />
      </CDropdownToggle>
      <CDropdownMenu className="pt-0">
        <CDropdownHeader className="bg-light fw-semibold py-2">
          Settings
        </CDropdownHeader>
        <CDropdownItem>
          <CIcon icon={cilUser} className="me-2" />
          {(user != null) ? (user['username']):("user")}
        </CDropdownItem>
        <CDropdownItem>
          <CIcon icon={cilEnvelopeClosed} className="me-2" />
          {(user != null) ? (user['email']):("email")}
        </CDropdownItem>
        <CDropdownDivider />
        <CDropdownItem onClick={handleLogoutButton} href="/login">
          <CIcon icon={cilAccountLogout} className="me-2" />
          Logout
        </CDropdownItem>
      </CDropdownMenu>
    </CDropdown>
  );
}