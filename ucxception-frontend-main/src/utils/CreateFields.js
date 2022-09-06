import React from "react";
import {
  CCol,
  CFormCheck,
  CFormInput,
  CFormFeedback,
  CFormLabel,
  CFormSelect,
  CInputGroup,
} from "@coreui/react";
import Select from "react-select";

export default function CreateFields() {
  function createInputNumber(
    index,
    size,
    labelText,
    objectName,
    inputName,
    isRequired,
    step,
    min,
    max,
    defaultValue,
    placeholderValue,
    setObject,
    hasError,
    errorMessage
  ) {
    return (
      <CCol key={index} className="my-2" md={size}>
        {labelText !== "" ? (
          <CFormLabel htmlFor={inputName}>
            <h6>{labelText}</h6>
          </CFormLabel>
        ) : null}
        <CFormInput
          type="number"
          name={objectName}
          id={inputName}
          required={isRequired}
          step={step}
          min={min}
          max={max}
          defaultValue={defaultValue}
          placeholder={placeholderValue}
          onChange={(e) =>
            setObject != null ? setObject(e.target.name, e.target.value) : null
          }
        />
        {hasError ? (
          <CFormFeedback invalid>{errorMessage}</CFormFeedback>
        ) : null}
      </CCol>
    );
  }

  function createInputText(
    index,
    size,
    labelText,
    objectName,
    inputName,
    isRequired,
    minLength,
    maxLength,
    defaultValue,
    placeholderValue,
    setObject,
    hasError,
    errorMessage
  ) {
    return (
      <CCol key={index} className="my-2" md={size}>
        {labelText !== "" ? (
          <CFormLabel htmlFor={inputName}>
            <h6>{labelText}</h6>
          </CFormLabel>
        ) : null}
        <CFormInput
          type="text"
          name={objectName}
          id={inputName}
          required={isRequired}
          minLength={minLength}
          maxLength={maxLength}
          defaultValue={defaultValue}
          placeholder={placeholderValue}
          onChange={(e) =>
            setObject != null ? setObject(e.target.name, e.target.value) : null
          }
        />
        {hasError ? (
          <CFormFeedback invalid>{errorMessage}</CFormFeedback>
        ) : null}
      </CCol>
    );
  }

  function createInputSelectHash(
    index,
    size,
    labelText,
    objectName,
    inputName,
    isRequired,
    placeholderValue,
    setObject,
    objectList,
    objectValue,
    objectDisplay
  ) {
    return (
      <CCol key={index} className="my-2" md={size}>
        {labelText !== "" ? (
          <CFormLabel htmlFor={inputName}>
            <h6>{labelText}</h6>
          </CFormLabel>
        ) : null}
        <CFormSelect
          name={objectName}
          id={inputName}
          required={isRequired}
          placeholder={placeholderValue}
          onChange={(e) =>
            setObject != null ? setObject(e.target.name, e.target.value) : null
          }
        >
          {objectList !== undefined && objectList !== null
            ? objectList.map((object, key) => (
                <option
                  className="text-primary"
                  key={key}
                  value={object[objectValue]}
                >
                  {object[objectDisplay]}
                </option>
              ))
            : null}
        </CFormSelect>
      </CCol>
    );
  }

  function createInputSelectList(
    index,
    size,
    labelText,
    objectName,
    inputName,
    isRequired,
    placeholderValue,
    setObject,
    objectList
  ) {
    return (
      <CCol key={index} className="my-2" md={size}>
        {labelText !== "" ? (
          <CFormLabel htmlFor={inputName}>
            <h6>{labelText}</h6>
          </CFormLabel>
        ) : null}
        <CFormSelect
          name={objectName}
          id={inputName}
          required={isRequired}
          placeholder={placeholderValue}
          onChange={(e) => (setObject != null ? setObject(e) : null)}
        >
          {objectList !== undefined && objectList !== null
            ? objectList.map((object, key) => (
                <option className="text-primary" key={key} value={object}>
                  {object}
                </option>
              ))
            : null}
        </CFormSelect>
      </CCol>
    );
  }

  function createSelect(
    index,
    size,
    labelText,
    objectName,
    inputName,
    isRequired,
    setObject,
    objectList
  ) {
    return (
      <CCol key={index} className="my-2" md={size}>
        {labelText !== "" ? (
          <CFormLabel htmlFor={inputName}>
            <h6>{labelText}</h6>
          </CFormLabel>
        ) : null}
        <CFormSelect
          name={objectName}
          id={inputName}
          required={isRequired}
          options={objectList}
          onChange={(e) =>
            setObject != null ? setObject(e.target.name, e.target.value) : null
          }
        ></CFormSelect>
      </CCol>
    );
  }

  function createMultipleDropdown(index, size, labelText, inputName, options) {
    return (
      <CCol key={index} className="my-2" md={size}>
        {labelText !== "" ? (
          <CFormLabel htmlFor={inputName}>
            <h6>{labelText}</h6>
          </CFormLabel>
        ) : null}
        <CInputGroup className="has-validation">
          <Select
            defaultValue={null}
            isMulti
            name={inputName}
            options={options}
            className="basic-multi-select"
            classNamePrefix="select"
          />
          <CFormFeedback invalid>Please choose a username.</CFormFeedback>
        </CInputGroup>
      </CCol>
    );
  }

  function createInputRadio(
    index,
    size,
    labelText,
    objectName,
    inputName,
    setObject,
    checked
  ) {
    return (
      <CCol key={index} className="my-2" md={size}>
        <CFormCheck
          name={objectName}
          id={inputName}
          checked={checked}
          label={labelText}
          onChange={(e) =>
            setObject != null
              ? setObject(e.target.name, e.target.checked)
              : null
          }
        />
      </CCol>
    );
  }

  function createInputFile(index, size, labelText, objectName, accept) {
    return (
      <CCol key={index} className="my-2" md={size}>
        {labelText !== "" ? (
          <CFormLabel>
            <h6>{labelText}</h6>
          </CFormLabel>
        ) : null}
        <CFormInput
          type="file"
          name={objectName}
          id={objectName}
          accept={accept}
          required
        />
      </CCol>
    );
  }

  return {
    createInputNumber,
    createInputText,
    createInputSelectHash,
    createInputSelectList,
    createInputRadio,
    createMultipleDropdown,
    createInputFile,
    createSelect,
  };
}
