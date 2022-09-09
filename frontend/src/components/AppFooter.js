import React from "react";
import { CFooter } from "@coreui/react";

const AppFooter = () => {
  return (
    <CFooter>
      <div className="ms-auto">
        <span className="me-1">Powered by</span>
        <a
          href="https://github.com/ucx-code/ucXception"
          target="_blank"
          rel="noopener noreferrer"
        >
          ucXception
        </a>
      </div>
    </CFooter>
  );
};

export default React.memo(AppFooter);
