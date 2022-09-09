import React from "react";
import { CDropdown, CDropdownToggle } from "@coreui/react";
import CIcon from "@coreui/icons-react";
import { cilHome } from "@coreui/icons";
import { Link } from "react-router-dom";

const AppHeaderHome = () => {
  return (
    <div variant="nav-item" alignment="end">
      <div placement="bottom-end" className="py-0">
        <Link to="/menu" style={{ color: "#768192" }}>
          <CIcon icon={cilHome} size="lg" className="my-1 mx-2" />
        </Link>
      </div>
    </div>
  );
};

export default AppHeaderHome;
