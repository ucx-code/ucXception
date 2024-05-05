import React from "react";
import CIcon from "@coreui/icons-react";
import { cilHome } from "@coreui/icons";
import { Link } from "react-router-dom";

const handleClick = () => {
  localStorage.removeItem("Copied");
};

const AppHeaderHome = () => {
  return (
    <div variant="nav-item" alignment="end">
      <div placement="bottom-end" className="py-0">
        <Link to="/menu" style={{ color: "#768192" }} onClick={handleClick}>
          <CIcon icon={cilHome} size="lg" className="my-1 mx-2" />
        </Link>
      </div>
    </div>
  );
};

export default AppHeaderHome;
