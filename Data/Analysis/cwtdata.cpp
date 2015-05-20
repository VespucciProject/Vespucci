#include "Data/Analysis/cwtdata.h"
CWTData::CWTData(QSharedPointer<VespucciDataset> dataset)
{
    dataset_ = dataset;
    peak_data_.set_size(dataset_->spectra_ptr()->n_cols);
}

///
/// \brief CWTData::Apply
/// \param wavelet
/// \param max_scale
/// \param gap_threshold
/// \param min_ridge_length
/// \param search_width
/// \param noise_threshold
/// \param noise_method
/// \param noise_window_size
/// \param save_coefs
/// \param save_coef_plots
/// \param save_ridge_plots
/// \param save_ridge_plot_data
/// \param save_directory
///
void CWTData::Apply(string wavelet,
                    uword max_scale,
                    uword gap_threshold,
                    uword min_ridge_length,
                    uword search_width,
                    double noise_threshold,
                    string noise_method,
                    uword noise_window_size,
                    bool save_coefs,
                    bool save_coef_plots,
                    bool save_ridge_plots,
                    bool save_ridge_plot_data,
                    bool estimate_width,
                    QString save_directory,
                    QString image_format,
                    QCPColorGradient gradient)
{
    uvec scales = conv_to<uvec>::from(linspace(1, max_scale, max_scale));

    mat *spectra = dataset_->spectra_ptr();
    vec abscissa = dataset_->wavelength();
    mat data;

    //iterate over spectra
    for (uword i = 0; i < spectra->n_cols; ++i){
        vec current_spectrum = spectra->col(i);
        mat coefs = Vespucci::Math::Transform::cwt(current_spectrum, wavelet, scales);


        //perform the analysis
        std::vector<Vespucci::Math::CWTRidge> ridges =
                Vespucci::Math::PeakFinding::FindRidges(coefs,
                                                        scales,
                                                        gap_threshold,
                                                        min_ridge_length,
                                                        search_width,
                                                        noise_threshold,
                                                        noise_method,
                                                        noise_window_size);
        //peform width estimation if requested
        if (estimate_width){
            Vespucci::Math::PeakFinding::EstimateWidth(current_spectrum, dataset_->wavelength(), ridges);

        }
        //
        data.set_size(ridges.size(), 4);
        std::for_each(ridges.begin(),
                      ridges.end(),
                      [&data](Vespucci::Math::CWTRidge &r)
                        {
                            rowvec row(4);
                            row(0) = r.PeakCenter();
                            row(1) = r.width();
                            row(2) = r.length();
                            row(3) = r.SNR();
                            data.insert_rows(data.n_rows, row);
                        });



        //save things if requested:

        //save coefficients if requested
        if (save_coefs){
            coefs.save(save_directory.toStdString() + "/coefs_" + std::to_string(i) + ".csv", csv_ascii);
        }
        //plot coefficients if requested
        if (save_coef_plots){
            QCPColorMapData *map_data =
                    new QCPColorMapData(coefs.n_cols,
                                        coefs.n_rows,
                                        dataset_->WavelengthRange(),
                                        QCPRange(1, coefs.n_cols));
            for (uword j1 = 0; j1 < coefs.n_cols; ++j1)
                for (uword i1 = 0; i1 < coefs.n_rows; ++i1)
                    map_data->setCell(j1, i1, coefs(i1,j1));
            QCustomPlot *map_plot = new QCustomPlot();
            QCPColorMap *color_map = new QCPColorMap(map_plot->xAxis, map_plot->yAxis);
            map_plot->addPlottable(color_map);
            color_map->setData(map_data);
            map_plot->xAxis->setLabel(dataset_->x_axis_description());
            map_plot->yAxis->setLabel("Scales");
            color_map->setGradient(gradient);
            color_map->rescaleDataRange(true);
            map_plot->rescaleAxes();
            map_plot->replot();

            //save the plot based on file format
            if (image_format == "tif")
                map_plot->saveRastered(save_directory + "/coef_plot_" + QString::number(i) + ".tif", 0, 0, 1.0, "TIFF");
            else if (image_format == "bmp")
                map_plot->saveBmp(save_directory + "/coef_plot_" + QString::number(i) + ".bmp");
            else if (image_format == "png")
                map_plot->savePng(save_directory + "/coef_plot_" + QString::number(i) + ".png");
            else if (image_format == "jpg")
                map_plot->saveJpg(save_directory + "/coef_plot_" + QString::number(i) + ".jpg");
            else
                map_plot->savePdf(save_directory + "/coef_plot_" + QString::number(i) + ".pdf");
        }//end if (plot)

        //save ridge info if requested
        if (save_ridge_plot_data || save_ridge_plots){
            QVector<double> ridge_plot_keys;
            QVector<double> ridge_plot_values;
            std::string current_column;
            std::ofstream ridge_plot_file;
            QCustomPlot *ridge_plot = 0;
            QCPGraph *ridge_graph = 0;

            if (save_ridge_plot_data){
                ridge_plot_file.open(save_directory.toStdString() + "/ridges.csv");
            }
            if (save_ridge_plots){
                ridge_plot = new QCustomPlot(0);
                ridge_plot->xAxis->setRange(dataset_->WavelengthRange());
                ridge_plot->yAxis->setRange(QCPRange(1, max_scale));
            }

            //iterate over ridges
            for (Vespucci::Math::CWTRidge r : ridges){
                //points.col(0) is index, points.col(1) is scale
                umat points = r.points();

                if (save_ridge_plots){
                    ridge_graph = ridge_plot->addGraph();
                    ridge_graph->setScatterStyle(QCPScatterStyle::ssDisc);
                }

                for (uword i1 = 0; i1 < points.n_rows; ++i1){

                    if (save_ridge_plot_data)
                        current_column += std::to_string(abscissa(points(i1, 0))) + "," + std::to_string(points(i1, 1)) + "\n";

                    if (save_ridge_plots){
                        ridge_plot_keys.append(abscissa(points(i1, 0)));
                        ridge_plot_values.append(points(i1, 1));
                    }

                } //end for over points

                if (save_ridge_plot_data){
                    ridge_plot_file << current_column;
                }
                if (save_ridge_plots){
                    //each ridge is considered a separate graph so that they may be linked
                    ridge_graph->addData(ridge_plot_keys, ridge_plot_values);
                }
            }//end for over ridges


            if (save_ridge_plots){
                //save the plot based on file format
                if (image_format == "tif")
                    ridge_plot->saveRastered(save_directory + "/ridge_plot_" + QString::number(i) + ".tif", 0, 0, 1.0, "TIFF");
                else if (image_format == "bmp")
                    ridge_plot->saveBmp(save_directory + "/ridge_plot_" + QString::number(i) + ".bmp");
                else if (image_format == "png")
                    ridge_plot->savePng(save_directory + "/ridge_plot_" + QString::number(i) + ".png");
                else if (image_format == "jpg")
                    ridge_plot->saveJpg(save_directory + "/ridge_plot_" + QString::number(i) + ".jpg");
                else
                    ridge_plot->savePdf(save_directory + "/ridge_plot_" + QString::number(i) + ".pdf");
            }

            if (ridge_plot_file.is_open()){
                ridge_plot_file.close();
            }

        } //end if for saving ridge info
    //add the collected information
    peak_data_(i) = data;

    }//end for iterating over spectra
}//end function