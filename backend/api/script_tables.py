def return_script():
	tables = [
		"""CREATE TABLE user (
			iduser	 INTEGER,
			public_id VARCHAR(10) UNIQUE NOT NULL,
			username	 VARCHAR(20) NOT NULL,
			email	 VARCHAR(255) UNIQUE NOT NULL,
			password	 VARCHAR(255) NOT NULL,
			PRIMARY KEY(iduser)
		);""",

		"""CREATE TABLE campaign (
			idcampaign	 INTEGER,
			type	 VARCHAR(512) NOT NULL,
			name	 VARCHAR(255) NOT NULL,
			state	 VARCHAR(255) NOT NULL,
			fipath	 VARCHAR(512),
			csvfilename VARCHAR(255),
			startdate	 DATE,
			enddate	 DATE,
			campaigntarget	 INTEGER,
			faultinjectortarget INTEGER,
			user_iduser INTEGER NOT NULL,
			PRIMARY KEY(idcampaign),
			FOREIGN KEY (user_iduser) REFERENCES user(iduser)
		);""",

		"""CREATE TABLE file (
			idfile		 INTEGER,
			name		 VARCHAR(512) NOT NULL,
			data		 BLOB NOT NULL,
			savedonstorage	 TINYINT NOT NULL,
			campaign_idcampaign INTEGER NOT NULL,
			PRIMARY KEY(idfile),
			FOREIGN KEY (campaign_idcampaign) REFERENCES campaign(idcampaign)
		);""",

		"""CREATE TABLE components (
			idcomponent			 INTEGER,
			name				 VARCHAR(255) NOT NULL,
			type				 VARCHAR(512) NOT NULL,
			component_type_idcomponenttype INTEGER NOT NULL,
			campaign_idcampaign		 INTEGER NOT NULL,
			PRIMARY KEY(idcomponent),
			FOREIGN KEY (component_type_idcomponenttype) REFERENCES component_type(idcomponentType),
			FOREIGN KEY (campaign_idcampaign) REFERENCES campaign(idcampaign)
		);""",

		"""CREATE TABLE component_type (
			idcomponenttype INTEGER,
			name		 VARCHAR(50) UNIQUE NOT NULL,
			PRIMARY KEY(idcomponenttype)
		);""",

		"""CREATE TABLE host (
			idhost		 INTEGER,
			type		 VARCHAR(255),
			domain		 VARCHAR(255),
			username		 VARCHAR(255),
			campaign_idcampaign INTEGER NOT NULL,
			PRIMARY KEY(idhost),
			FOREIGN KEY (campaign_idcampaign) REFERENCES campaign(idcampaign)
		);""",

		"""CREATE TABLE component_parameter (
			idparameter		 INTEGER,
			name			 VARCHAR(255) NOT NULL,
			data			 BLOB NOT NULL,
			components_idcomponent INTEGER NOT NULL,
			PRIMARY KEY(idparameter),
			FOREIGN KEY (components_idcomponent) REFERENCES components(idcomponent)
		);""",

		"""CREATE TABLE execution (
			idexecution	 INTEGER,
			name		 VARCHAR(512) NOT NULL,
			hasfault		 INTEGER NOT NULL,
			ntargetruns	 INTEGER DEFAULT 0,
			ncurrentruns	 INTEGER DEFAULT 0,
			campaign_idcampaign INTEGER NOT NULL,
			PRIMARY KEY(idexecution),
			FOREIGN KEY (campaign_idcampaign) REFERENCES campaign(idcampaign)
		);""",

		"""CREATE TABLE campaign_parameter (
			idparameter	 INTEGER,
			name		 VARCHAR(255) NOT NULL,
			data		 BLOB NOT NULL,
			campaign_idcampaign INTEGER NOT NULL,
			PRIMARY KEY(idparameter),
			FOREIGN KEY (campaign_idcampaign) REFERENCES campaign(idcampaign)
		);""",

		"""CREATE TABLE components_components (
			components_idcomponent	 INTEGER,
			components_idcomponent1 INTEGER NOT NULL,
			PRIMARY KEY(components_idcomponent),
			FOREIGN KEY (components_idcomponent) REFERENCES components(idcomponent),
			FOREIGN KEY (components_idcomponent1) REFERENCES components(idcomponent)
		);""",

		"""CREATE TABLE components_host (
			components_idcomponent INTEGER,
			host_idhost		 INTEGER NOT NULL,
			PRIMARY KEY(components_idcomponent),
			FOREIGN KEY (components_idcomponent) REFERENCES components(idcomponent),
			FOREIGN KEY (host_idhost) REFERENCES host(idhost)
		);""",

		"""CREATE TABLE reset_password (
			id		 INTEGER,
			expiration_time DATETIME NOT NULL,
			hashed_token	 VARCHAR(512) NOT NULL,
			user_iduser	 INTEGER NOT NULL,
			PRIMARY KEY(id),
			FOREIGN KEY (user_iduser) REFERENCES user(iduser)
		);"""
	]
		
	return tables