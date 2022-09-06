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
import { cilAccountLogout, cilUser, cilCheckCircle } from "@coreui/icons";
import CIcon from "@coreui/icons-react";

export default function AppHeaderDropdown() {
  const { handleLogout } = useContext(AuthContext);
  const { addAlert } = useContext(AlertsContext);

  const handleLogoutButton = () => {
    handleLogout();
    addAlert("Success logout", "success", cilCheckCircle);
  };

  return (
    <CDropdown variant="nav-item" alignment="end">
      <CDropdownToggle placement="bottom-end" className="py-0" caret={false}>
        <CIcon icon={cilUser} size="lg" className="my-1 mx-2" />
      </CDropdownToggle>
      <CDropdownMenu className="pt-0">
        <CDropdownHeader className="bg-light fw-semibold py-2">
          Settings
        </CDropdownHeader>
        {/* <CDropdownItem href="/menu">
          <CIcon icon={cilUser} className="me-2" />
          Profile
        </CDropdownItem> 
        <CDropdownDivider />*/}
        <CDropdownItem onClick={handleLogoutButton} href="/login">
          <CIcon icon={cilAccountLogout} className="me-2" />
          Logout
        </CDropdownItem>
      </CDropdownMenu>
    </CDropdown>
  );
}
