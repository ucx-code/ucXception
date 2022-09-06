import React from "react";
import { CContainer, CHeader, CHeaderNav } from "@coreui/react";
import { AppHeaderHome, AppHeaderDropdown } from "./header/index";

const AppHeader = () => {
  return (
    <CHeader position="sticky" className="mb-4">
      <CContainer fluid>
        <CHeaderNav className="d-none d-md-flex me-auto">
          <h1>ucXception</h1>
        </CHeaderNav>
        <CHeaderNav>
          <AppHeaderHome />
          <AppHeaderDropdown />
        </CHeaderNav>
      </CContainer>
    </CHeader>
  );
};

export default AppHeader;
