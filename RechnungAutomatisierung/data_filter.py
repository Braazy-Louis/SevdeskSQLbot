import logging
from datetime import datetime

class DataFilter:
    """
    Class to filter and extract data from the database for use in form inputs.
    """
    
    # Field mappings for different form types
    FIELD_MAPPINGS = {
        "Abmeldung Ihrer Wohnung": {
            'email': '5',
            'phone_number': '4',
            'nachricht': '7',  # This will determine which set of meta_keys to use
            
            # Mappings for "sonstige Gründe zutreffen."
            'sonstige_grunde': {
                'gender': '94.2',
                'first_name': '94.3',
                'last_name': '94.6',
                'street': '29.1',
                'house_number': '29.2',
                'postal_code': '29.5',
                'city': '29.3',
                'country': '29.6',
                'birth_date': '28',
                'contribution_number': '26'
            },
            
            # Mappings for "ich dauerhaft ins Ausland ziehe."
            'ins_ausland': {
                'gender': '95.2',
                'first_name': '95.3',
                'last_name': '95.6',
                'street': '33.1',
                'house_number': '33.2',
                'postal_code': '33.5',
                'city': '33.3',
                'country': '33.6',
                'birth_date': '32',
                'move_date': '34',
                'contribution_number': '30'
            },
            
            # Mappings for "ich zu einem anderen Beitragszahler ziehe."
            'anderer_beitragszahler': {
                'gender': '96.2',
                'first_name': '96.3',
                'last_name': '96.6',
                'street': '39.1',
                'house_number': '39.2',
                'postal_code': '39.3',
                'city': '39.5',
                'country': '39.6',
                'birth_date': '41',
                'move_date': '40',
                'contribution_number': '35'
            }
        },
        "Kontaktformular (Sonstiges)": {
            'gender': '101.2',
            'first_name': '101.3',
            'last_name': '101.6',
            'street': '79.1',
            'house_number': '79.2',
            'city': '79.3',
            'postal_code': '79.5',
            'country': '79.6',
            'email': '5',
            'phone_number': '4',
            'contribution_number': '80',
            'nachricht': '77'  # Use meta_key 77 for nachricht field
        },
        "Anmeldung Ihrer Wohnung": {
            'gender': '96.2',
            'first_name': '96.3',
            'last_name': '96.6',
            'street': '39.1',
            'house_number': '39.2',
            'postal_code': '39.3',
            'city': '39.5',
            'email': '5',
            'phone_number': '4',
            'birth_date': '41',
            'move_date': '40'
        },
        "Änderung Ihrer Daten": {
            'gender': '98.2',
            'first_name': '98.3',
            'last_name': '98.6',
            'street': '42.1',
            'house_number': '42.2',
            'postal_code': '42.3',
            'city': '42.5',
            'email': '5',
            'phone_number': '4',
            'birth_date': '43',
            'contribution_number': '44'
        },
        "Befreiung vom Rundfunkbeitrag": {
            'gender': '99.2',
            'first_name': '99.3',
            'last_name': '99.6',
            'street': '45.1',
            'house_number': '45.2',
            'postal_code': '45.3',
            'city': '45.5',
            'email': '5',
            'phone_number': '4',
            'birth_date': '46',
            'contribution_number': '47'
        },
        "Widerspruch": {
            'gender': '100.2',
            'first_name': '100.3',
            'last_name': '100.6',
            'street': '48.1',
            'house_number': '48.2',
            'postal_code': '48.3',
            'city': '48.5',
            'email': '5',
            'phone_number': '4',
            'birth_date': '49',
            'contribution_number': '50'
        }
    }
    
    @staticmethod
    def extract_customer_data(customer_data):
        """
        Extract and format customer data from the database for use in form inputs.
        
        Args:
            customer_data (dict): Dictionary containing customer data from the database
            
        Returns:
            dict: Formatted customer data ready for use in form inputs
        """
        try:
            # Initialize the result dictionary with default values
            formatted_data = {
                'gender': 'Herr',  # Default gender
                'first_name': '',
                'last_name': '',
                'street': '',
                'house_number': '',
                'postal_code': '',
                'city': '',
                'country': '',
                'email': '',
                'phone_area': '',
                'phone_number': '',
                'birth_date': '',
                'contribution_number': '',
                'move_date': '',
                'form_type': '',
                'nachricht': '',
                'additional_info': ''
            }
            
            # First, get the form type
            form_type = customer_data.get('1', '')
            formatted_data['form_type'] = form_type
            
            logging.info(f"Processing form type: {form_type}")
            
            if form_type == "Abmeldung Ihrer Wohnung":
                reason = customer_data.get('7', '').strip()
                
                if reason == "sonstige Gründe zutreffen.":
                    mapping_set = DataFilter.FIELD_MAPPINGS[form_type]['sonstige_grunde']
                    pflegeheim_text = customer_data.get('25', '')
                    beitragsnummer = customer_data.get('26', '')
                    formatted_data['nachricht'] = (
                        f"Ich begründe die Abmeldung der Wohnung folgendermaßen: {pflegeheim_text}, "
                        f"meine Beitragsnummer: {beitragsnummer}"
                    )
                    logging.debug(f"Using combined Pflegeheim text and Beitragsnummer for nachricht: {formatted_data['nachricht']}")
                
                elif reason == "ich dauerhaft ins Ausland ziehe.":
                    mapping_set = DataFilter.FIELD_MAPPINGS[form_type]['ins_ausland']
                    beitragsnummer = customer_data.get('30', '')
                    formatted_data['nachricht'] = (
                        f"Ich begründe die Abmeldung der Wohnung folgendermaßen: {reason}, "
                        f"meine Beitragsnummer: {beitragsnummer}"
                    )
                    logging.debug(f"Using combined reason and Beitragsnummer for nachricht: {formatted_data['nachricht']}")
                
                elif reason == "ich zu einem anderen Beitragszahler ziehe.":
                    mapping_set = DataFilter.FIELD_MAPPINGS[form_type]['anderer_beitragszahler']
                    # Get the contribution number from meta_key 35
                    beitragsnummer = customer_data.get('35', '')
                    # Get the roommate's name from meta_keys 36.3 and 36.6
                    mitbewohner_vorname = customer_data.get('36.3', '')
                    mitbewohner_nachname = customer_data.get('36.6', '')
                    
                    # Format the complete message
                    formatted_data['nachricht'] = (
                        f"Ich begründe die Abmeldung der Wohnung folgendermaßen: {reason}\n"
                        f"Beitragsnummer vom Mitbewohner: {beitragsnummer}\n"
                        f"Name vom Mitbewohner: {mitbewohner_vorname} {mitbewohner_nachname}"
                    )
                    logging.debug(f"Using formatted message with Mitbewohner details for nachricht: {formatted_data['nachricht']}")
                
                if mapping_set:
                    # Map the fields using the appropriate set
                    for field, meta_key in mapping_set.items():
                        if meta_key in customer_data:
                            formatted_data[field] = customer_data[meta_key]
                            logging.debug(f"Mapped {meta_key} to {field}: {customer_data[meta_key]}")
                    
                    # Handle common fields
                    for field in ['email', 'phone_number']:
                        meta_key = DataFilter.FIELD_MAPPINGS[form_type][field]
                        if meta_key in customer_data:
                            formatted_data[field] = customer_data[meta_key]
                else:
                    logging.error(f"Unknown Abmeldung reason: {reason}")
            else:
                # Handle other form types as before...
                field_mapping = DataFilter.FIELD_MAPPINGS.get(form_type)
                if field_mapping:
                    for field, meta_key in field_mapping.items():
                        if meta_key in customer_data:
                            formatted_data[field] = customer_data[meta_key]
            
            # Special handling for phone number (split into area code and number)
            if formatted_data['phone_number']:
                phone = formatted_data['phone_number'].strip()
                # Try to split the phone number into area code and number
                if phone.startswith('0'):
                    # German phone numbers typically start with 0
                    # Find the first non-digit character or split after 4-5 digits
                    split_index = 4  # Default split after 4 digits
                    for i in range(4, min(6, len(phone))):
                        if not phone[i].isdigit():
                            split_index = i
                            break
                    
                    formatted_data['phone_area'] = phone[:split_index]
                    formatted_data['phone_number'] = phone[split_index:].strip()
            
            # Log the extracted data
            logging.info(f"Extracted customer data: {formatted_data}")
            
            return formatted_data
            
        except Exception as e:
            logging.error(f"Error extracting customer data: {e}")
            return None
    
    @staticmethod
    def format_date(date_str):
        """
        Format date string from database format to form format.
        
        Args:
            date_str (str): Date string in database format (YYYY-MM-DD)
            
        Returns:
            str: Formatted date string (DD.MM.YYYY)
        """
        try:
            if not date_str:
                return ""
                
            # Parse the date string
            date_obj = datetime.strptime(date_str, "%Y-%m-%d")
            
            # Format the date as DD.MM.YYYY
            return date_obj.strftime("%d.%m.%Y")
        except Exception as e:
            logging.error(f"Error formatting date {date_str}: {e}")
            return date_str
    
    @staticmethod
    def get_form_type_description(form_type):
        """
        Get a human-readable description for the form type.
        
        Args:
            form_type (str): Form type code from the database
            
        Returns:
            str: Human-readable description of the form type
        """
        form_types = {
            "Abmeldung Ihrer Wohnung": "Wohnungsabmeldung",
            "Kontaktformular (Sonstiges)": "Sonstiges",
            "Anmeldung Ihrer Wohnung": "Wohnungsanmeldung",
            "Änderung Ihrer Daten": "Datenänderung",
            "Befreiung vom Rundfunkbeitrag": "Befreiung",
            "Widerspruch": "Widerspruch"
        }
        
        return form_types.get(form_type, form_type) 