import fire
from redshift import RedShift

rs_engine = RedShift()

class ABDataFetcher(object):
	"""An Ab data fetcher"""

	def fetch_ab_log_events(self, experiment_id):
		'''
		Given an test id. Fetch all ABLogEvents
		that correspond to the input experiment_id.

		Save the output into a csv file. The columns 
		are:

		I. src_uuid_tx
		II. dw_site_visitor_id
		II. ab_variant_id
		III. ab_log_event_utc_ts
		'''
		#Construct Query
		AB_LOG_EVENT_QUERY = """
		SELECT src_uuid_tx
		       , dw_site_visitor_id
		       , ab_log_event_utc_ts
		       , ab_variant_id
		FROM dw_report.dw_ab_log_event_f
		WHERE ab_test_id={}
		ORDER BY ab_log_event_utc_ts;
		""".format(experiment_id)
		#execute query
		ab_log_events = rs_engine.submit(AB_LOG_EVENT_QUERY)
		ab_log_events.to_csv('ab_log_events.csv')
		print ab_log_events

	def fetch_eee(self,experiment_id):
		'''
		Given a test id. Fetch all EEEEvents 
		that correspond to the input experiment_id.

		Save output into a csv file. The columns are: 

		I. dw_site_visitor_id
		II. variant_id
		III. expmt_expsr_utc_ts
		'''
		#Construct Query
		EEE_QUERY = """
		SELECT dw_site_visitor_id
		       , variant_id
		       , expmt_expsr_utc_ts
		FROM dw_report.dw_expmt_expsr_event_f
		WHERE test_id={};
		""".format(experiment_id)
		#Execute query
		experiment_exposure_events = rs_engine.submit(EEE_QUERY)
		experiment_exposure_events.to_csv('experiment_exposure_events.csv')
		print experiment_exposure_events

	def get_sponsored_clicks(self, vertical, date, month, year):
		'''
		Given a vertical and a date, fetch all
		sponsored click events.

		Save output into a csv file where the columns are:

		I. dw_site_visitor_id
		II. click_utc_ts
		'''
		SPONSORED_CLICK_QUERY = """
		SELECT dw_site_visitor_id
		       , click_utc_ts
		FROM dw_views.dw_clicks_enriched
		WHERE is_sponsored_in = 1
		  AND page_hier_lvl1_nm = '{vertical}'
		  AND dw_suspected_bot_in = 'False'
		  AND dw_eff_dt > '{dt}'
		""".format(
			vertical=vertical,
			dt='{0}-{1}-{2}'.format(year,month,date)
		)

		#execute query
		click_events = rs_engine.submit(SPONSORED_CLICK_QUERY)
		click_events.to_csv('click_events.csv')
		print click_events

if __name__=='__main__':
	fire.Fire(ABDataFetcher)