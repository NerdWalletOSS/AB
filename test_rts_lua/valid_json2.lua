return [=[
{
	"id": "1",
	"name": "T1",
	"api_id": "1",
	"request_webapp_id": "2",
	"test_type_id": "1",
	"bin_type_id": "4",
	"pred_id": null,
	"channel_id": null,
	"description": "New Description",
	"is_dev_specific": "0",
	"has_filters": "0",
	"seed": "5582638522457145545",
	"external_id": "151787373911388600",
	"created_at": "2018-02-05 15:35:39",
	"t_create": "151787373911388600",
	"updated_at": "2018-02-05 15:35:39",
	"t_update": "151787373913040700",
	"creator_id": "1",
	"updater_id": "1",
	"state_id": "1",
	"Creator": "joe",
	"TestType": "ABTest",
	"BinType": "c_to_v_ok_v_to_c_ok_v_to_v_not_ok",
	"State": "draft",
	"Variants": [{
		"id": "1",
		"name": "Control",
		"api_id": "1",
		"request_webapp_id": "2",
		"description": null,
		"test_id": "1",
		"percentage": "50",
		"is_final": "0",
		"url": null,
		"custom_data": null,
		"created_at": "0000-00-00 00:00:00",
		"t_create": "0",
		"updated_at": "2018-02-05 15:35:39",
		"t_update": "151787373913040700"
	}, {
	"id": "2",
	"name": "Variant_A",
	"api_id": "1",
	"request_webapp_id": "2",
	"description": null,
	"test_id": "1",
	"percentage": "30",
	"is_final": "0",
	"url": null,
	"custom_data": null,
	"created_at": "0000-00-00 00:00:00",
	"t_create": "0",
	"updated_at": "2018-02-05 15:35:39",
	"t_update": "151787373913040700"
	}, {
	"id": "3",
	"name": "Variant_B",
	"api_id": "1",
	"request_webapp_id": "2",
	"description": null,
	"test_id": "1",
	"percentage": "20",
	"is_final": "0",
	"url": null,
	"custom_data": null,
	"created_at": "0000-00-00 00:00:00",
	"t_create": "0",
	"updated_at": "2018-02-05 15:35:39",
	"t_update": "151787373913040700"
	}],
	"DeviceCrossVariant": {
		"Desktop": [{
			"id": "1",
      "device_id": "1",
			"variant_id": "1",
			"test_id": "1",
			"percentage": "100"
		}, {
		"id": "2",
		"api_id": "1",
		"request_webapp_id": "1",
		"device_id": "1",
		"variant_id": "2",
		"test_id": "1",
		"ramp_num": "1",
		"percentage": "0"
		}, {
		"id": "3",
		"api_id": "1",
		"request_webapp_id": "1",
		"device_id": "1",
		"variant_id": "3",
		"test_id": "1",
		"ramp_num": "1",
		"percentage": "0"
		}],
		"Mobile_iOS": [{
			"id": "4",
			"api_id": "1",
			"request_webapp_id": "1",
			"device_id": "2",
			"variant_id": "1",
			"test_id": "1",
			"ramp_num": "1",
			"percentage": "10"
		}, {
		"id": "5",
		"api_id": "1",
		"request_webapp_id": "1",
		"device_id": "2",
		"variant_id": "2",
		"test_id": "1",
		"ramp_num": "1",
		"percentage": "20"
		}, {
		"id": "6",
		"api_id": "1",
		"request_webapp_id": "1",
		"device_id": "2",
		"variant_id": "3",
		"test_id": "1",
		"ramp_num": "1",
		"percentage": "70"
		}],
		"Mobile_Android": [{
			"id": "7",
			"api_id": "1",
			"request_webapp_id": "1",
			"device_id": "3",
			"variant_id": "1",
			"test_id": "1",
			"ramp_num": "1",
			"percentage": "30"
		}, {
		"id": "8",
		"api_id": "1",
		"request_webapp_id": "1",
		"device_id": "3",
		"variant_id": "2",
		"test_id": "1",
		"ramp_num": "1",
		"percentage": "30"
		}, {
		"id": "9",
		"api_id": "1",
		"request_webapp_id": "1",
		"device_id": "3",
		"variant_id": "3",
		"test_id": "1",
		"ramp_num": "1",
		"percentage": "40"
		}],
		"Tablet_iOS": [{
			"id": "10",
			"api_id": "1",
			"request_webapp_id": "1",
			"device_id": "4",
			"variant_id": "1",
			"test_id": "1",
			"ramp_num": "1",
			"percentage": "40"
		}, {
		"id": "11",
		"api_id": "1",
		"request_webapp_id": "1",
		"device_id": "4",
		"variant_id": "2",
		"test_id": "1",
		"ramp_num": "1",
		"percentage": "40"
		}, {
		"id": "12",
		"api_id": "1",
		"request_webapp_id": "1",
		"device_id": "4",
		"variant_id": "3",
		"test_id": "1",
		"ramp_num": "1",
		"percentage": "20"
		}],
		"Tablet_Android": [{
			"id": "13",
			"api_id": "1",
			"request_webapp_id": "1",
			"device_id": "5",
			"variant_id": "1",
			"test_id": "1",
			"ramp_num": "1",
			"percentage": "50"
		}, {
		"id": "14",
		"api_id": "1",
		"request_webapp_id": "1",
		"device_id": "5",
		"variant_id": "2",
		"test_id": "1",
		"ramp_num": "1",
		"percentage": "50"
		}, {
		"id": "15",
		"api_id": "1",
		"request_webapp_id": "1",
		"device_id": "5",
		"variant_id": "3",
		"test_id": "1",
		"ramp_num": "1",
		"percentage": "0"
		}],
		"Other": [{
			"id": "16",
			"api_id": "1",
			"request_webapp_id": "1",
			"device_id": "6",
			"variant_id": "1",
			"test_id": "1",
			"ramp_num": "1",
			"percentage": "60"
		}, {
		"id": "17",
		"api_id": "1",
		"request_webapp_id": "1",
		"device_id": "6",
		"variant_id": "2",
		"test_id": "1",
		"ramp_num": "1",
		"percentage": "4"
		}, {
		"id": "18",
		"api_id": "1",
		"request_webapp_id": "1",
		"device_id": "6",
		"variant_id": "3",
		"test_id": "1",
		"ramp_num": "1",
		"percentage": "36"
		}]
	}
}]=]