import React from "react";
import { useLocation } from "react-router-dom";
import { CRow, CCol, CBreadcrumb, CBreadcrumbItem } from "@coreui/react";
import breadcrumbs_routes from "../routes";
import "@coreui/coreui/dist/css/coreui.min.css";

const AppBreadcrumb = () => {
  const currentLocation = useLocation().pathname;

  return (
    <CRow className="justify-content-evenly">
      <CCol sm={breadcrumbs_routes.length} className="text-center">
        <CBreadcrumb style={{ "--cui-breadcrumb-divider": "'>'" }}>
          {breadcrumbs_routes.map((breadcrumb, index) => {
            return (
              <CBreadcrumbItem
                {...(breadcrumb.pathname === currentLocation
                  ? null
                  : { active: true })}
                key={index}
              >
                {breadcrumb.name}
              </CBreadcrumbItem>
            );
          })}
        </CBreadcrumb>
      </CCol>
    </CRow>
  );
};

export default React.memo(AppBreadcrumb);
